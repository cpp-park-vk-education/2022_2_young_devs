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


#include "game_room.h"
#include "tasks.h"
#include "room_builder.h"

#include <gtest/gtest.h>

#include "config.h"

#include "threadsave_queue.h"

#include "helpers_func.h"
#include "shared_func.h"



static size_t PLAYER_ID = 0;
static size_t ROOM_ID 	= 0;


#include "cond_var.h"
extern std::condition_variable cond_var;

std::mutex tmp_mutex;

using namespace std::string_literals;

struct RoomTasks
{
	GameRoom *room;
	const size_t room_id;
	ThreadsafeQueue<Task *> tasks;
};

std::vector<RoomTasks> vecRoomsTasks;

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

GameRoom *CreateRoomById(size_t room_id, std::string const filename)
{
    if (!filename.empty())
    {
        T_TestOutput::SetName(filename);
    }
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(room_id)\
		.withGame(TypeGame::ST)\
		.withPlayers(0, 1)\
		.withOutput(new T_TestOutput)\
		.build();
	return room;
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
		assert(candidates.size() != 0);
		result = candidates[rand() % candidates.size()];
	}
	else
	{
		// ход первый, доступна любая ячейка
		result = rand() % 81;
	}
	field.Set(result, player_index == 0 ? TypeCell::X : TypeCell::O);
	return result;
}

void FillTuple(std::list<std::tuple<bool, size_t, ST_Field>> &rooms)
{
	size_t _id = 0;
	for (auto &[is_removed, id, field] : rooms)
	{
		is_removed = false;
		id = _id++; 
	}
}

void GenerateStressTest(size_t case_number, size_t count_rooms)
{
	// std::srand(std::time(0));
	std::ofstream file((BASE_DIR / "tests/test_files/test_case_"s + std::to_string(case_number) + ".test"s).data());
	std::vector<ssize_t> prev_steps(count_rooms, -1);
	std::list<std::tuple<bool, size_t, ST_Field>> rooms(count_rooms);
	FillTuple(rooms);
	size_t ready_rooms = 0;

	for (size_t i = 0; i < 81; ++i)
	{
		size_t index_field = 0;
		for (auto &[is_removed, id, field] : rooms)
		{
			if (!is_removed)
			{
				size_t room_id 			= id;
				size_t player_index 	= i % 2;
				size_t step 			= getRandomValidStep(field, prev_steps[id], player_index);
				prev_steps[id]			= step;
				file << room_id << " s " <<  player_index << " " << step << "\n";
				if (field.IsEnd().isEnd)
				{
					is_removed = true;
					++ready_rooms;
				}
			}
			++index_field;
		}
		if (ready_rooms == count_rooms)
		{
			return;
		}
	}
}


void ThreadRoutine(size_t room_id)
{	
	auto optional_room = findRoomById(vecRoomsTasks, room_id);
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
						!vecRoomsTasks[room_id].tasks.empty()
					) 
						|| room->finish;
			});
			

			if (!room->finish)
			{
				Task *task = nullptr;
				vecRoomsTasks[room_id].tasks.try_pop(task);
				// assert(task);
                if (!task)
                {
                    continue;
                }
				(*task)();
				delete task;
			}
		}
		// else поток выходит из функции и завершает выполнение
        /*
            {
                std::lock_guard locker(tmp_mutex);
                std::cout << "Game in room \033[32m" << std::setw(3) << room_id << "\033[0m completed." << "\n";
            }
        */
	}
}


void StressTest(size_t case_number, std::string const &filename)
{
    vecRoomsTasks.clear();
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

	std::ifstream file((BASE_DIR / "tests/test_files/test_case_"s + std::to_string(case_number) + ".test"s).data());
	while (file >> room_id >> command >> player_id >> value)
	{
		auto optional_room 	= findRoomById(vecRoomsTasks, room_id);
		GameRoom *room;
		if (!optional_room)
		{
			room = CreateRoomById(room_id, filename);
			vecRoomsTasks.push_back({ .room = room, .room_id = room_id});
			threads.push_back(std::thread(ThreadRoutine, room_id));
		}
		else
		{
			room = *optional_room;
		}
		switch(command)
		{
			case 's':
			{
				Task *task = new T_StepTask(room, player_id == 0 ? players[0] : players[1], value);
				vecRoomsTasks[room_id].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			case 'r':
			{
				Task *task = new T_RollbackTask(room, player_id == 0 ? players[0] : players[1], value);
				vecRoomsTasks[room_id].tasks.push(task);
				cond_var.notify_one();
				break;
			}
			default:
			{
				std::cout << "comamnd unknown\n";
			}
		}
		// для синхронизации и дебага
		// std::this_thread::sleep_for(std::chrono::microseconds(1000));
	}
	for (size_t i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
}

TEST(TestStress, DISABLED_TestGenerateStressCases)
{
    // <test_case_number>, <count_rooms>
    GenerateStressTest(2, 15);
    GenerateStressTest(3, 100);
    GenerateStressTest(4, 1000);
}

TEST(TestStress, TestStress_15)
{
    // <test_case_number> <filename>
	StressTest(2, "stress_test_1_15_rooms.out");
	EXPECT_TRUE(true);
}

TEST(TestStress, TestStress_100)
{
	StressTest(3, "stress_test_1_100_rooms.out");
	EXPECT_TRUE(true);
}

TEST(TestStress, TestStress_1000)
{
	StressTest(4, "stress_test_1_1000_rooms.out");
	EXPECT_TRUE(true);
}