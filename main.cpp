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


void func(GameRoom *room)
{
	std::cout << "hello" << std::endl;
}


int main()
{
	size_t room_id = 0;
	TypeGame typeGame = TypeGame::ST;

	T_Room *room = GameRoomBuilder()\
				.withRoomId(room_id)\
				.withGame(typeGame)\
				.withPlayers(0)\
				.withOutput(new T_StreamOutput)\
				.build();

	// std::vector<StepInfo> steps = {
	// 	{ .player_id = 0, .index = 1, .cell = TypeCell::X},
	// 	{ .player_id = 1, .index = 5, .cell = TypeCell::O},
	// 	{ .player_id = 0, .index = 50, .cell = TypeCell::X},
	// 	{ .player_id = 1, .index = 2, .cell = TypeCell::O},
	// };

	// room->Initialize(steps);
	

	GameLoop_Bot(room);

	// GameLoop_Bot(room);
	return 0;
}




// std::vector<std::ofstream> files;
// for (size_t i = 0; i < COUNT_ROOMS; ++i)
// {
// 	std::string fileName = BASE_DIR / "tests/test_files/test"s + std::to_string(i) + ".out"s;
// 	files.emplace_back(std::ofstream{fileName.data()});
// 	vecRoomsTasks.push_back({ .room = CreateRoom(std::move(files[i])), .room_id = i});
// }
// работает, но после в функции Output при попытке записать в поток - seg fault


struct StepInfo
{
    ssize_t     player_id;
    size_t      index;
    TypeCell    cell;
};

std::vector<StepInfo> getSteps(nlohmann::json &json)
{
	std::vector<StepInfo> vec;
	for (size_t i = 0; i < json["steps"].size(); ++i)
	{	
		TypeCell cell = TypeCell::E;
		size_t c = json["cell"][i]["cell"];
		if (c == -1)
		{
			cell =  TypeCell::O;
		}
		else if (c == 1)
		{
			cell =  TypeCell::X;
		}
		vec.push_back(
			{.player_id = json["steps"][i]["player_id"]},
			{.index = json["steps"][i]["index"]},
			{.index = cell },
		)
	}
	return vec;
}