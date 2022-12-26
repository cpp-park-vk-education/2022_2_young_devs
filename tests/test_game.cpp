#include <iostream>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <functional>
#include <optional>
#include <type_traits>

// т.к. либы header-only, подключение в cmake не требуется
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "game_room.h"
#include "tasks.h"
#include "room_builder.h"

#include <gtest/gtest.h>

#include "config.h"

using namespace std::string_literals;

std::string operator/(std::string const &left, std::string const &right)
{
	return left + "/" + right;
}

const std::string _BASE_DIR = BASE_DIR;

std::optional<GameRoom *> findById(std::vector<GameRoom *> &items, size_t id)
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

std::optional<Player> findById(std::vector<Player> &items, size_t id)
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

static size_t PLAYER_ID = 0;
static size_t ROOM_ID 	= 0;
// static size_t FILE_NO = 1;
static size_t COUNT_ROOMS = 2;

// GameRoom *CreateRoom(std::ofstream &&out, bool withBot = false)
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

GameRoom *CreateRoom(std::string const &file_name, bool withBot = false)
{
	size_t player_id_1 = PLAYER_ID++;
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(ROOM_ID++)\
		.withGame(TypeGame::ST)\
		// .withPlayers(player_id_1, withBot ? -1 : PLAYER_ID++)
		.withPlayers(0, 1)\
		.withOutput(new T_StreamOutput(file_name))\
		.build();
		// .withLogging(false)
	return room;
}

void StressTest()
{
	// std::vector<std::ofstream &&> vecFiles;
	// std::vector<std::ofstream> streams;
	// std::vector<GameRoom *> rooms;
	// for (size_t i = 0; i < COUNT_ROOMS; ++i)
	// {
		// std::string fileName = BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s;
		// streams.emplace_back(fileName.data() );

		// std::ofstream *file = new std::ofstream((BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s).data());
		// vecFiles.push_back(
		// 	std::ofstream{
		// 			(BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s).data()
		// 		}
		// 	);
		// rooms.push_back(CreateRoom(fileName));
	// }

	std::ofstream file1((BASE_DIR / "tests/test_files/test1.out"s).data());
	std::ofstream file2((BASE_DIR / "tests/test_files/test2.out"s).data());
	rooms.push_back(CreateRoom(file1));
	rooms.push_back(CreateRoom(file2));

	// вектор игроков
	std::vector<Player> players;
	for (auto &&room : rooms)
	{
		std::cout << "room_id = " << room->id << "\n";
		for (auto &&player : room->GetPlayers())
		{
			players.push_back(player);
			std::cout << "\tplayer_id = " << player.id << "\n";
		}
	}

	size_t 	room_id;
	char 	command;
	size_t 	player_id;
	size_t 	value;

	boost::asio::thread_pool pool(4);
	// std::istream &file = std::cin;
	std::ifstream file((BASE_DIR / "tests/test_files/test_case_1.test"s).data());
	puts("))");
	while (file >> room_id >> command >> player_id >> value)
	{
		auto opt_room 	= findById(rooms, room_id);
		auto opt_player = findById(players, player_id);
		if (!opt_room || !opt_player)
		{
			std::cout << "Room or Player does not exist\n";
			continue;
		}
		GameRoom *room = *opt_room;
		Player player  = *opt_player;
		std::cout << command << std::endl;
		switch(command)
		{
			case 's':
			{
				T_StepTask task(room, player, value);
				boost::asio::post(pool, task);
				break;
			}
			case 'r':
			{
				T_RollbackTask task(room, player, value);
				boost::asio::post(pool, task);
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
}

TEST(TestSuite, Test_1)
{
	std::cout << BASE_DIR << std::endl;
	StressTest();
	EXPECT_TRUE(true);
}