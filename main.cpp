#include <iostream>

// т.к. либы header-only, подключение в cmake не требуется
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "game_room.h"
#include "tasks.h"

int main()
{
	T_GameField *field 	= new OT_Field(4);
	T_GameLogic *logic 	= new OT_Logic;
	T_Output 	*output = new T_StreamOutput;
	T_Bot       *bot 	= new OT_Bot;
	Player player_1 	= { .id = 0, .isBot = false, .cell = TypeCell::X };
	Player player_2 	= { 		 .isBot = true,  .cell = TypeCell::O };
	GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot);

	size_t value;
	size_t player_id;
	char command;
	std::cout << "Enter player_id, index\n";

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
				std::cout << "comamnd unknown\n";
			}
		}
	}
	return 0;
}