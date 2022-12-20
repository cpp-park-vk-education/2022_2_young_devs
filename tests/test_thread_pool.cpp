#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <functional>
#include <optional>
#include <type_traits>
#include <cstdlib>
#include <chrono>
#include <queue>
#include <condition_variable>

// т.к. либы header-only, подключение в cmake не требуется
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "game_room.h"
#include "tasks.h"
#include "room_builder.h"

#include <gtest/gtest.h>

#include "config.h"
#include "shared_func.h"

using namespace std::string_literals;


static std::vector<GameRoom *> rooms;


std::optional<GameRoom *> _findRoomById(std::vector<GameRoom *> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item->id == id)
		{
			return item;
		}
	}
	return std::nullopt;
}

GameRoom *CreateRoom_WithOutput(size_t room_id, std::ostream &out)
{
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(room_id)\
		.withGame(TypeGame::ST)\
		.withPlayers(0, 1)\
		.withOutput(new T_StreamOutput(out))\
		.build();
	return room;
}


void ThreadPoolTest(size_t case_number, std::string const &filename, bool sleep = false)
{
	rooms.clear();
	
	std::vector<Player> players = {
		{ .id = 0, .isBot = false, .cell = TypeCell::X }, 
		{ .id = 1, .isBot = false, .cell = TypeCell::O }
	};

	size_t 	room_id;
	char 	command;
	size_t 	player_id;
	size_t 	value;

	boost::asio::thread_pool pool(4);

	// std::istream &file = std::cin;
	std::ifstream file((BASE_DIR / "tests/test_files/test_case_"s + std::to_string(case_number) + ".test"s).data());

	size_t n = 0;
	while (file >> room_id >> command >> player_id >> value)
	{
		if (n++ % 100 == 0)
		{
			std::cout << n << "\n";
		}
		auto optional_room 	= _findRoomById(rooms, room_id);
		GameRoom *room;
		if (!optional_room)
		{
			room = CreateRoomById(room_id, filename);
			rooms.push_back(room);
		}
		else
		{
			room = *optional_room;
		}
		switch(command)
		{
			case 's':
			{
				T_StepTask task(room, player_id == 0 ? players[0] : players[1], value);
				boost::asio::post(pool, task);
				break;
			}
			case 'r':
			{
				T_RollbackTask task(room, player_id == 0 ? players[0] : players[1], value);
				boost::asio::post(pool, task);
				break;
			}
			default:
			{
				std::cout << "comamnd unknown\n";
			}
		}
		if (sleep)
		{
			// для синхронизации, чтобы "игроки" не опережали друг друга
			std::this_thread::sleep_for(std::chrono::microseconds(500));
		}
	}
}

// 500 - 100
// 300 - 96
// 100 - 93
// 10 - 91

TEST(TestThreadPool, DISABLED_TestThreadPool_Synchronization)
{
	// <test_case>, <filename out>, <sync / no sync>
	ThreadPoolTest(1, "1_thread_pool_rooms_2_sync.out", true);
	EXPECT_TRUE(true);
}

TEST(TestThreadPool, DISABLED_TestThreadPool_NoSynchronization)
{
	ThreadPoolTest(1, "2_thread_pool_rooms_2_no_sync.out", false);
	EXPECT_TRUE(true);
}

TEST(TestThreadPool, DISABLED_TestThreadPool_ManyRooms_Synchronization)
{
	ThreadPoolTest(3, "3_thread_pool_rooms_100_sync.out", true);
	EXPECT_TRUE(true);
}

TEST(TestThreadPool, DISABLED_TestThreadPool_ManyRooms_NoSynchronization)
{
	ThreadPoolTest(3, "4_thread_pool_rooms_100_no_sync.out", false);
	EXPECT_TRUE(true);
}

TEST(TestThreadPool, TestThreadPool_Stress_100)
{
	auto start = std::chrono::steady_clock::now();

	ThreadPoolTest(3, "thread_pool_100_rooms_no_sync.out", false);

	showTime(start, "TestThreadPool_Stress_100");

	EXPECT_TRUE(true);
}

TEST(TestThreadPool, DISABLED_TestThreadPool_Stress_1000)
{
	auto start = std::chrono::steady_clock::now();

	ThreadPoolTest(4, "thread_pool_1000_rooms_no_sync.out", false);

	showTime(start, "TestThreadPool_Stress_1000");

	EXPECT_TRUE(true);
}


// для полный отчетов

/*
	std::ofstream files[COUNT_ROOMS] = {};
	for (size_t i = 0; i < COUNT_ROOMS; ++i)
	{
		std::string fileName = BASE_DIR / "tests/test_files/"s + dir + "test_room_"s + std::to_string(i) + ".out"s;
		files[i].open(fileName);
		rooms.push_back(CreateRoom_WithOutput(i, files[i]));
	}
*/
