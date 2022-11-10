#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "compressor.h"
#include "game_loop.h"

extern std::mutex mutex;
extern std::condition_variable cv;

extern ssize_t indexGlobal;
extern ssize_t needsInputIndexPlayer;
extern bool continueLoop;
extern bool endGame;

/*
	1. Если мы вводим индексы с консоли
		Игровой цикл ожидает ввода в методе MakeStep
	2. Если сервер инициирует ввод индексов (по начажатю пользователя на ячейку в графическом интерфейсе)
		Игра должна продолжаться по этому событию

	Событийное программирование?


	Игровой цикл крутится в отдельном потоке
	Когда необходим ввод пользователя, поток засыпает

	Происходит ввод пользователя
	Функция обработчик будет поток игрового цикла
*/

/*
	Задачи:
		- Play(continueGame) не отрабатывает уже завершенную игру
		- убрать Undo() из Input и запоминание индексов
		- сохранять состояния в объекте Field
			- IGameField::operator[] -> int IGameField::At(index) + &int IGameField::At(index, indexPlayer)
			- уметь откатываться, откатываться на определенный шаг
			- гетер количества шагов
			- (в вк описание функционала)
		- ТЕСТЫ
*/

void UserEvents(size_t indexPlayer)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (needsInputIndexPlayer != indexPlayer && !endGame)
		{
			cv.wait(lock);
		}
		if (endGame)
		{
			break;
			lock.unlock();
		}
		std::cout << "Player " << indexPlayer << ", enter index of cell\n";
		uint16_t indexLocal;
		std::cin >> indexLocal;
		needsInputIndexPlayer = -1;
		indexGlobal = indexLocal;
		continueLoop = true;
		cv.notify_all();
	}
}

void Go()
{
	// std::thread user0(UserEvents, 0);
	// std::thread user1(UserEvents, 1);

	// *********** [ NETEST ] ***********

	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();
	// IInput *inputPlayer0 = new InputEvent(0);
	// IInput *inputPlayer1 = new InputEvent(1);
	{
		IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
								   .withGameLogic(gameLogic)
								   .withInputPlayerOne(new InputConsole)
								   .withInputPlayerTwo(new InputBot(1, field))
								   .withOutput(new OutputConsole)
								   .build();

		ticTacToe->Play();
	}

	// std::cout << "*********[  Game ended   ]*********\n\n";
	// std::cout << "*********[ Continue game ]*********\n\n";

	// ManagerTicTacToeField manager;
	// std::string strField = manager.CompressField(gameLogic->GetField());
	// std::cout << "Field of last game: " << strField << '\n';
	delete gameLogic;

	// // заполняем поле через строку
	// IGameField *field = manager.DecompressField(strField);

	// {
	// 	IGameLogic *gameLogic = new TicTacToeLogic(3, field);

	// 	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
	// 							   .withGameLogic(gameLogic)
	// 							   .withInputPlayerOne(inputPlayer0)
	// 							   .withInputPlayerTwo(inputPlayer1)
	// 							   .withOutput(new OutputConsole)
	// 							   .build();

	// 	// начинаем игру с прошлого состояния
	// 	// нет обработки, если игра уже была выиграна
	// 	ticTacToe->Play(false);

	// 	delete gameLogic;
	// }

	// delete inputPlayer0;
	// delete inputPlayer1;

	// delete field;

	std::cout << "Waiting user...\n";
	// user0.join();
	// user1.join();
	std::cout << "User completed!\n";
}

int main()
{
	Go();
	return 0;
}
