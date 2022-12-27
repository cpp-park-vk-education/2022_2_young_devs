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
#include <chrono>


#include "game_room.h"
#include "tasks.h"
#include "room_builder.h"

#include <gtest/gtest.h>

#include "config.h"

#include "threadsave_queue.h"

#include "helpers_func.h"
#include "shared_func.h"


using namespace std::string_literals;

const std::string _BASE_DIR = BASE_DIR;

static std::vector<GameRoom *> rooms;


void ConsistentTest(size_t case_number, std::string const &filename)
{
    rooms.clear();

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
                task();
				break;
			}
			case 'r':
			{
                T_RollbackTask task(room, player_id == 0 ? players[0] : players[1], value);
                task();
				break;
			}
			default:
			{
				std::cout << "comamnd unknown\n";
			}
		}
	}
}

void showTime(auto start, std::string &&message)
{
    auto end = std::chrono::steady_clock::now();
    std::cout << message << " " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}


TEST(TestConsistent, TestConsistent_15)
{
    // <test_case_number> <filename>
    auto start = std::chrono::steady_clock::now();

	ConsistentTest(2, "consistent_test_15_rooms.out");

    showTime(start, "TestConsistent_15");

	EXPECT_TRUE(true);
}

TEST(TestConsistent, TestConsistent_100)
{
    auto start = std::chrono::steady_clock::now();

	ConsistentTest(3, "consistent_test_100_rooms.out");

    showTime(start, "TestConsistent_100");

	EXPECT_TRUE(true);
}

TEST(TestConsistent, TestConsistent_1000)
{
    auto start = std::chrono::steady_clock::now();

	ConsistentTest(4, "consistent_test_1000_rooms.out");

    showTime(start, "TestConsistent_1000");

	EXPECT_TRUE(true);
}