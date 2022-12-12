#include <iostream>
#include <unistd.h>
#include <fstream>

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

int main()
{
	// КОММЕНТАРИИ - ИГРА С БОТОМ
	T_GameField *field 	= new ST_Field;
	T_GameLogic *logic 	= new ST_Logic;
	T_Output 	*output = new T_StreamOutput;
	// T_Bot       *bot 	= new OT_Bot;
	// T_Bot       *bot 	= new ST_Bot;
	Player player_1 	= { .id = 0, .isBot = false,  .cell = TypeCell::X };
	Player player_2 	= { .id = 1, .isBot = false,  .cell = TypeCell::O };
	// Player player_bot 	= {          .isBot = true,   .cell = TypeCell::O };
	// GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot, TypeGame::ST);
	GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, nullptr, TypeGame::ST);

	size_t value;
	size_t player_id;
	char command;
	std::cout << "Enter player_id, index\n";

	boost::asio::thread_pool pool(4);

	// 	ТЕСТ КЕЙС
	std::ifstream file("test.txt");
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
				std::cout << "comamnd unknown\n";
			}
		}
		sleep(1);
	}
	return 0;
}