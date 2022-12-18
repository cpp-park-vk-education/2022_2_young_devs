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


/*
	[OT = Ordinary Tic Tac Toe]
	[ST = Strategic Tic Tac Toe]
	Задачи
		+ декомпозировать 
		+ структурировать отчеты об ошибке
		+ протестить стратегические с ботом
		- mutex на игровое поле
		- perf тестирование
		- minimax для бота OT
		- ML для бота ST
*/


/*
	Задачи по игровой логике:
		1. GameRoom *room = new T_Room(...)
			Новый объект room должен создаваться каждый раз при создании новой игровой сессии
			Инициализироваться реальными значениями (room_id, player_id_1, player_id_2)
		2. Stop() синхронный?
		3. Как определяется, кто играет первым? Если игрок хочет, чтобы бот играл первым?
*/

/*
	- Можно ли в векторе хранит объекты, в полях которых есть std::mutex
	- Нужен билдер для команты
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
	GameResult result = room->GetResult();
	std::cout << "result.isEnd = " << result.isEnd << "\n";
	std::cout << "result.winnerCell = " << result.winnerCell << "\n";
	std::cout << "result.winner = " << result.winner.id << "\n";
	std::cout << "result.draw = " << result.draw << "\n";
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