#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <functional>
#include <optional>
#include <type_traits>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <numeric>
#include <list>

// т.к. либы header-only, подключение в cmake не требуется
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "game_room.h"
#include "tasks.h"
#include "room_builder.h"

#include <gtest/gtest.h>

#include "config.h"

#include "cond_var.h"

#include "helpers_func.h"

using namespace std::string_literals;


static size_t PLAYER_ID = 0;
static size_t ROOM_ID 	= 0;

static const size_t COUNT_ROOMS = 2;

extern std::condition_variable cond_var;

std::mutex tmp_mutex;



/*
	[OT = Ordinary Tic Tac Toe]
	[ST = Strategic Tic Tac Toe]
	Задачи
		+ декомпозировать 
		+ структурировать отчеты об ошибке
		+ протестить стратегические с ботом
		+ mutex на игровое поле
		+ perf тестирование
		- minimax для бота OT
		- ML для бота ST
*/


#if 0

void OutputHelp()
{
	std::cout << "comand : s = step, r = rollback\n";
	std::cout << "index  : 0 - 80 (ST), 0 - 8 (OT\n";
	std::cout << "Enter <command> <index>\n";
}

void GameLoop_Bot(GameRoom *room)
{
	std::vector<Player> players = room->GetPlayers();
	Player player_1 = players[0];
	Player player_2 = players[1];

	size_t value;
	char command;

	OutputHelp();
	
	boost::asio::thread_pool pool(4);

	while (std::cin >> command >> value)
	{
		switch(command)
		{
			case 's':
			{
				T_StepTask task(room, player_1, value);
				boost::asio::post(pool, task);
				break;
			}
			case 'r':
			{
				T_RollbackTask task(room, player_1, value);
				boost::asio::post(pool, task);
				break;
			}
			default:
			{
				std::cout << "Comamnd unknown\n";
			}
		}
	}
}

void GameLoop_OtherPlayer(GameRoom *room)
{
	std::vector<Player> players = room->GetPlayers();
	Player player_1 = players[0];
	Player player_2 = players[1];

	size_t value;
	size_t player_id;
	char command;
	std::cout << "Enter <command> <player_id> <index>\n";

	OutputHelp();

	boost::asio::thread_pool pool(4);

	// 	ТЕСТ КЕЙС
	std::ifstream file("../test_files/test.txt");
	// std::istream &file = std::cin;
	while (file >> command >> player_id >> value)
	{
		switch(command)
		{
			case 's':
			{
				if (player_id == 0)
				{
					T_StepTask task(room, player_1, value);
					boost::asio::post(pool, task);
				}
				else
				{
					T_StepTask task(room, player_2, value);
					boost::asio::post(pool, task);
				}
				break;
			}
			case 'r':
			{
				if (player_id == 0)
				{
					T_RollbackTask task(room, player_1, value);
					boost::asio::post(pool, task);
				}
				else
				{
					T_RollbackTask task(room, player_2, value);
					boost::asio::post(pool, task);
				}
				break;
			}
			default:
			{
				std::cout << "Comamnd unknown\n";
			}
		}
		sleep(1);
	}
}



int main()
{
	size_t room_id = 0;
	TypeGame typeGame = TypeGame::ST;

	T_GameField *field 	= new ST_Field;
	T_GameLogic *logic  = new ST_Logic;
	T_Bot       *bot  	= new ST_Bot;

	T_Output 	*output = new T_StreamOutput;
	Player player_1 	= { .id = 0, .isBot = false,  .cell = TypeCell::X };
	Player player_2 	= { /* -1 */ .isBot = true,   .cell = TypeCell::O };

	// Игра с ботом
	GameRoom *room 		= new T_Room(room_id, player_1, player_2, field, logic, output, bot, typeGame);

	// GameLoop_Bot(room);
	return 0;
}

#elif 1

template <typename T>
class ThreadsafeQueue
{
    std::queue<T>           _queue;
    mutable std::mutex      _queue_mutex;
    std::condition_variable _queue_waiting_condition;

public:

    ThreadsafeQueue() = default;
    ThreadsafeQueue(ThreadsafeQueue const& other)
    {
        std::lock_guard<std::mutex> locker(other._queue_mutex);
        _queue = other._queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        _queue.push(std::move(new_value));
        _queue_waiting_condition.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> locker(_queue_mutex);
        _queue_waiting_condition.wait(locker,[this]{return !_queue.empty();});
        value = std::move(_queue.front());
        _queue.pop();
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
    
        if (_queue.empty())
            return false;
    
        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        return _queue.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        return _queue.size();
    }
};

struct RoomTasks
{
	GameRoom *room;
	const size_t room_id;
	ThreadsafeQueue<Task *> tasks;
};

std::vector<RoomTasks> roomsWithTasks;

std::string operator/(std::string const &left, std::string const &right)
{
	return left + "/" + right;
}

const std::string _BASE_DIR = BASE_DIR;

std::optional<GameRoom *> findRoomById(std::vector<RoomTasks> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item.room->id == id)
		{
			return item.room;
		}
	}
	return std::nullopt;
}

std::optional<Player> findPlayerById(std::vector<Player> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item.id == id)
		{
			return item;
		}
	}
	return std::nullopt;
}


GameRoom *CreateRoom(std::ostream &out, bool withBot = false)
{
	size_t player_id_1 = PLAYER_ID++;
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(ROOM_ID++)\
		.withGame(TypeGame::ST)\
		// .withPlayers(player_id_1, withBot ? -1 : PLAYER_ID++)
		.withPlayers(0, 1)\
		// .withOutput(new T_StreamOutput(std::move(out)))
		.withOutput(new T_NoOutput)\
		.build();
	return room;
}

GameRoom *CreateRoom2(size_t room_id)
{
	size_t player_id_1 = PLAYER_ID++;
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(room_id)\
		.withGame(TypeGame::ST)\
		// .withPlayers(player_id_1, withBot ? -1 : PLAYER_ID++)
		.withPlayers(0, 1)\
		// .withOutput(new T_StreamOutput(std::move(out)))
		.withOutput(new T_NoOutput)\
		.build();
	return room;
}


void Output(T_GameField &field)
{
    auto convertToBlocks = [](size_t index){
        size_t i = index / 9;
        size_t j = index % 9;
        size_t start_index_in_block = ((i / 3) * 3 + (j / 3)) * 9;
        return start_index_in_block + (i % 3) * 3 + (j % 3);
    };

    for (size_t i = 0; i < field.Size(); ++i)
    {
        size_t index = convertToBlocks(i);
        std::string strCell = TypeCellStr(field.At(index));
		std::cout << std::setw(2) << strCell << ' ';
		if (i % field.Dimension() == field.Dimension() - 1)
		{
			std::cout << '\n';
		}
    }
}

size_t getRandomValidStep(T_GameField &field, ssize_t prev_step, size_t player_index)
{
	size_t result = 0;
	if (prev_step != -1)
	{
		size_t needed_block = prev_step % 9;
		std::vector<size_t> candidates;
		for (size_t i = 0; i < 9; ++i)
		{
			if (field.At(needed_block * 9 + i) == TypeCell::E)
			{
				candidates.push_back(needed_block * 9 + i);
			}
		}
		// если нужное поле заполнено, ходим в любую доступную ячейку
		if (candidates.size() == 0)
		{
			for (size_t i = 0; i < 81; ++i)
			{
				if (field.At(i) == TypeCell::E)
				{
					candidates.push_back(i);
				}	
			}
		}
		// std::cout << "candidates.size() = " << candidates.size() << std::endl;
		// Output(field);
		// std::cout << "end? " << field.IsEnd().isEnd << "\n";
		// if (candidates.size() == 0)
		// {
			// exit(0);
		// }
		result = candidates[rand() % candidates.size()];
	}
	else
	{
		// ход первый
		result = rand() % 81;
	}
	field.Set(result, player_index == 0 ? TypeCell::X : TypeCell::O);
	return result;
}

void Fill(std::list<std::tuple<bool, size_t, ST_Field>> &lst)
{
	size_t _id = 0;
	for (auto &[is_removed, id, field] : lst)
	{
		is_removed = false;
		id = _id++; 
	}
}

void GenerateTestCase(size_t count_rooms, size_t steps_in_rooms)
{
	// std::srand(std::time(0));
	std::ofstream file((BASE_DIR / "tests/test_files/test_case_2.test"s).data());
	std::vector<ssize_t> 			prev_steps(count_rooms, -1);
	std::list<std::tuple<bool, size_t, ST_Field>> fields(count_rooms);
	Fill(fields);
	size_t ready_rooms = 0;

	for (size_t i = 0; i < steps_in_rooms; ++i)
	{
		size_t index_field = 0;
		puts("this5");
		for (auto &[is_removed, id, field] : fields)
		{
			if (!is_removed)
			{
				size_t room_id 		= id;
				std::cout << "ROOM_ID +++++++++ = " << room_id << "\n";
				size_t player_index = i % 2;
				size_t step 		= getRandomValidStep(field, prev_steps[id], player_index);
				prev_steps[id]		= step;
				file << room_id << " s " <<  player_index << " " << step << "\n";
				if (field.IsEnd().isEnd)
				{
					Output(field);
					std::cout << "room_done = " << room_id << " draw = " << field.IsEnd().draw << std::endl;
					is_removed = true;
					++ready_rooms;
				}
			}
			++index_field;
		}
		if (ready_rooms == count_rooms)
		{

			std::cout << "DONEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
			return;
		}
		std::cout << ">>>>>>>> READY_ROOMS = " << ready_rooms << std::endl;
		std::cout << ">>>>>>>> I =           " << i << std::endl;
	}
}

bool hasRoomId(size_t id)
{
	for (size_t i = 0; i < roomsWithTasks.size(); ++i)
	{
		if (roomsWithTasks[i].room_id == id)
		{
			return true;
		}
	}
	return false;
}

size_t indexFromId(size_t id)
{
	for (size_t i = 0; i < roomsWithTasks.size(); ++i)
	{
		if (roomsWithTasks[i].room_id == id)
		{
			return i;
		}
	}
	return 0;
}

void ThreadRoutine(size_t room_id)
{	
	auto optional_room = findRoomById(roomsWithTasks, room_id);
	if (optional_room)
	{	
		T_Room *room = dynamic_cast<T_Room *>(*optional_room);
		while (!room->finish)
		{
			std::unique_lock<std::mutex> locker(room->mutex);
			cond_var.wait(locker, [room, room_id]()
			{
				return (
						!room->busy && 
						hasRoomId(room_id) &&
						!roomsWithTasks[indexFromId(room_id)].tasks.empty()
						// !roomsWithTasks[room_id].tasks.empty()
					) 
						|| room->finish;
			});
			

			if (!room->finish)
			{
				Task *task = nullptr;
				roomsWithTasks[indexFromId(room_id)].tasks.try_pop(task);
				// roomsWithTasks[room_id].tasks.try_pop(task);
				assert(task);
				(*task)();
				delete task;
			}
		}
		// else поток выходит из функции и завершает выполнение
		{
			std::lock_guard locker(tmp_mutex);
			std::cout << "Game in room " << room_id << " completed." << "\n";
			std::cout << "The room was serviced by thread " << std::this_thread::get_id() << "\n";
		}
	}
}

void Test2Rooms()
{
	std::ofstream files[COUNT_ROOMS] = {};
	for (size_t i = 0; i < COUNT_ROOMS; ++i)
	{
		std::string fileName = BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s;
		files[i].open(fileName);
		roomsWithTasks.push_back({ .room = CreateRoom(files[i]), .room_id = i});
	}

	// вектор игроков
	std::vector<Player> players = {
		{ .id = 0, .isBot = false, .cell = TypeCell::X }, 
		{ .id = 1, .isBot = false, .cell = TypeCell::O }
	};

	size_t 	room_id;
	char 	command;
	size_t 	player_id;
	size_t 	value;

	std::vector<std::thread> threads;
	for (size_t i = 0; i < COUNT_ROOMS; ++i)
	{
		threads.push_back(std::thread(ThreadRoutine, i));
	}

	std::ifstream file((BASE_DIR / "tests/test_files/test_case_1.test"s).data());
	while (file >> room_id >> command >> player_id >> value)
	{
		auto opt_room 	= findRoomById(roomsWithTasks, room_id);
		auto opt_player = findPlayerById(players, player_id);
		if (!opt_room || !opt_player)
		{
			std::cout << "Room or Player does not exist\n";
			continue;
		}
		GameRoom *room = *opt_room;
		Player player  = *opt_player;
		switch(command)
		{
			case 's':
			{
				Task *task = new T_StepTask(room, player, value);
				roomsWithTasks[indexFromId(room_id)].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			case 'r':
			{
				Task *task = new T_RollbackTask(room, player, value);
				roomsWithTasks[indexFromId(room_id)].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			default:
			{
				std::cout << "comamnd unknown\n";
			}
		}
		// для синхронизации, чтобы "игроки" не опережали друг друга
		// std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
	for (size_t i = 0; i < COUNT_ROOMS; ++i)
	{
		// threads.push_back(std::thread(ThreadRoutine, i));
		threads[i].join();
	}
}

void StressTest()
{
	// вектор игроков
	std::vector<Player> players = {
		{ .id = 0, .isBot = false, .cell = TypeCell::X }, 
		{ .id = 1, .isBot = false, .cell = TypeCell::O }
	};

	size_t 	room_id;
	char 	command;
	size_t 	player_id;
	size_t 	value;

	std::vector<std::thread> threads;

	std::ifstream file((BASE_DIR / "tests/test_files/test_case_2.test"s).data());
	while (file >> room_id >> command >> player_id >> value)
	{
		auto opt_room 	= findRoomById(roomsWithTasks, room_id);
		auto opt_player = findPlayerById(players, player_id);
		GameRoom *room;
		if (!opt_room)
		{
			room = CreateRoom2(room_id);
			roomsWithTasks.push_back({ .room = room, .room_id = room_id});
			threads.push_back(std::thread(ThreadRoutine, room_id));
		}
		else
		{
			room = *opt_room;
		}
		Player player  = *opt_player;
		switch(command)
		{
			case 's':
			{
				Task *task = new T_StepTask(room, player, value);
				roomsWithTasks[indexFromId(room_id)].tasks.push(task);
				// roomsWithTasks[room_id].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			case 'r':
			{
				Task *task = new T_RollbackTask(room, player, value);
				roomsWithTasks[indexFromId(room_id)].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			default:
			{
				std::cout << "comamnd unknown\n";
			}
		}
		// для синхронизации, чтобы "игроки" не опережали друг друга
		// std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
	for (size_t i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
}

int main()
{
	std::cout << BASE_DIR << std::endl;
	// достаточно 81 хода, чтобы игра завершилась
	GenerateTestCase(15, 81);
	StressTest();
	puts("END MAIN");
}
#endif






// std::vector<std::ofstream> files;
// for (size_t i = 0; i < COUNT_ROOMS; ++i)
// {
// 	std::string fileName = BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s;
// 	files.emplace_back(std::ofstream{fileName.data()});
// 	roomsWithTasks.push_back({ .room = CreateRoom(std::move(files[i])), .room_id = i});
// }
// работает, но после в функции Output при попытке записать в поток - seg fault