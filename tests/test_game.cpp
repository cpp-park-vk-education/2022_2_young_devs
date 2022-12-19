// #include <iostream>
// #include <unistd.h>
// #include <fstream>
// #include <thread>
// #include <functional>
// #include <optional>
// #include <type_traits>
// #include <cstdlib>
// #include <queue>
// #include <condition_variable>

// // т.к. либы header-only, подключение в cmake не требуется
// #include <boost/asio/thread_pool.hpp>
// #include <boost/asio/post.hpp>

// #include "game_room.h"
// #include "tasks.h"
// #include "room_builder.h"

#include <gtest/gtest.h>

// #include "config.h"

// using namespace std::string_literals;


// static size_t PLAYER_ID = 0;
// static size_t ROOM_ID 	= 0;
// // static size_t FILE_NO = 1;
// static const size_t COUNT_ROOMS = 2;

// std::condition_variable cond_var;



// #include <queue>
// #include <mutex>
// #include <condition_variable>


// template <typename T>
// class ThreadsafeQueue
// {
//     std::queue<T>           _queue;
//     mutable std::mutex      _queue_mutex;
//     std::condition_variable _queue_waiting_condition;

// public:

//     ThreadsafeQueue() = default;
//     ThreadsafeQueue(ThreadsafeQueue const& other)
//     {
//         std::lock_guard<std::mutex> locker(other._queue_mutex);
//         _queue = other._queue;
//     }
//     void push(T new_value)
//     {
//         std::lock_guard<std::mutex> locker(_queue_mutex);
//         _queue.push(std::move(new_value));
//         _queue_waiting_condition.notify_one();
//     }
//     void wait_and_pop(T& value)
//     {
//         std::unique_lock<std::mutex> locker(_queue_mutex);
//         _queue_waiting_condition.wait(locker,[this]{return !_queue.empty();});
//         value = std::move(_queue.front());
//         _queue.pop();
//     }
//     bool try_pop(T& value)
//     {
//         std::lock_guard<std::mutex> locker(_queue_mutex);
    
//         if (_queue.empty())
//             return false;
    
//         value = std::move(_queue.front());
//         _queue.pop();
//         return true;
//     }

//     bool empty() const
//     {
//         std::lock_guard<std::mutex> locker(_queue_mutex);
//         return _queue.empty();
//     }

//     size_t size() const
//     {
//         std::lock_guard<std::mutex> locker(_queue_mutex);
//         return _queue.size();
//     }
// };




// std::string operator/(std::string const &left, std::string const &right)
// {
// 	return left + "/" + right;
// }

// const std::string _BASE_DIR = BASE_DIR;

// std::optional<GameRoom *> findById(std::vector<GameRoom *> &items, size_t id)
// {
// 	for (auto &&item : items)
// 	{
// 		if (item->id == id)
// 		{
// 			return item;
// 		}
// 	}
// 	return std::nullopt;
// }

// std::optional<Player> findById(std::vector<Player> &items, size_t id)
// {
// 	for (auto &&item : items)
// 	{
// 		if (item.id == id)
// 		{
// 			return item;
// 		}
// 	}
// 	return std::nullopt;
// }


// GameRoom *CreateRoom(std::ostream &out = std::cout, bool withBot = false)
// {
// 	size_t player_id_1 = PLAYER_ID++;
// 	GameRoom *room = GameRoomBuilder()\
// 		.withRoomId(ROOM_ID++)\
// 		.withGame(TypeGame::ST)\
// 		// .withPlayers(player_id_1, withBot ? -1 : PLAYER_ID++)
// 		.withPlayers(0, 1)\
// 		.withOutput(new T_StreamOutput(out))\
// 		.build();
// 		// .withLogging(false)
// 	return room;
// }

// void GenerateTestCase(size_t count_rooms)
// {
// 	// std::vector
// 	std::ofstream file((BASE_DIR / "tests/test_files/test_case_2.test"s).data());
// 	for (size_t i = 0; i < count_rooms * 100; ++i)
// 	{
// 		size_t room_id 	= rand() % count_rooms;
// 		size_t index 	= rand() % 81;
// 		size_t player_1 = rand() % 2;
// 		size_t steps
// 		file << 
// 	}
// }

// std::vector<RoomTasks> roomsWithTasks;

// struct RoomTasks
// {
// 	std::atomic<bool> finish = false;
// 	const size_t room_id;
// 	ThreadsafeQueue<Task *> tasks;
// };

// size_t indexFromId(size_t id)
// {
// 	for (size_t i = 0; i < roomsWithTasks.size(); ++i)
// 	{
// 		if (room.room_id == id)
// 		{
// 			return id;
// 		}

// 	}

// }

// std::vector<GameRoom *> rooms;

// void Waiting(size_t room_id)
// {	
// 	auto opt_room = findById(rooms, room_id);
// 	if (opt_room)
// 	{	
// 		GameRoom *room = *opt_room;
// 		std::unique_lock<std::mutex> locker(room.mutex);
// 		cond_var.wait(locker, [room]()
// 		{
// 			return (
// 					!room.busy && 
// 					!roomsWithTasks[indexFromId(room_id)].tasks.empty()
// 				) 
// 				|| roomsWithTasks[indexFromId(room_id)].finish;
// 		});
// 		if (!tasks[indexFromId(room_id)].finish)
// 		{
// 			Task *task = nullptr;
// 			roomsWithTasks[indexFromId(room_id)].tasks.try_pop(task);
// 			assert(task);
// 			task();
// 			delete task;
// 		}
// 		// else поток выходит из зоны видимости и завершается
// 	}
// }


// void StressTest()
// {
// 	std::vector<std::ofstream> streams;
// 	std::ofstream files[COUNT_ROOMS] = {};
// 	for (size_t i = 0; i < COUNT_ROOMS; ++i)
// 	{
// 		std::string fileName = BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s;
// 		files[i].open(fileName);
// 		rooms.push_back(CreateRoom(files[i]));
// 	}

// 	// вектор игроков
// 	std::vector<Player> players;
// 	for (auto &&room : rooms)
// 	{
// 		std::cout << "room_id = " << room->id << "\n";
// 		for (auto &&player : room->GetPlayers())
// 		{
// 			players.push_back(player);
// 			std::cout << "\tplayer_id = " << player.id << "\n";
// 		}
// 	}

// 	size_t 	room_id;
// 	char 	command;
// 	size_t 	player_id;
// 	size_t 	value;

// 	boost::asio::thread_pool pool(4);
// 	// std::istream &file = std::cin;
// 	std::ifstream file((BASE_DIR / "tests/test_files/test_case_1.test"s).data());
// 	while (file >> room_id >> command >> player_id >> value)
// 	{
// 		auto opt_room 	= findById(rooms, room_id);
// 		auto opt_player = findById(players, player_id);
// 		if (!opt_room || !opt_player)
// 		{
// 			std::cout << "Room or Player does not exist\n";
// 			continue;
// 		}
// 		GameRoom *room = *opt_room;
// 		Player player  = *opt_player;
// 		switch(command)
// 		{
// 			case 's':
// 			{
// 				T_StepTask task(room, player, value);
// 				boost::asio::post(pool, task);
// 				break;
// 			}
// 			case 'r':
// 			{
// 				T_RollbackTask task(room, player, value);
// 				boost::asio::post(pool, task);
// 				break;
// 			}
// 			default:
// 			{
// 				std::cout << "comamnd unknown\n";
// 			}
// 		}
// 		// для синхронизации, чтобы "игроки" не опережали друг друга
// 		std::this_thread::sleep_for(std::chrono::microseconds(1000));
// 	}
// }

TEST(TestSuite, Test_1)
{
	// std::cout << BASE_DIR << std::endl;
	// StressTest();
	EXPECT_TRUE(true);
}