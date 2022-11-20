#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "compressor.h"
#include "game_loop.h"
#include "game_progress.h"

#include "config.h"

extern std::mutex mutex;
extern std::condition_variable cv;

extern ssize_t indexGlobal;
extern ssize_t needsInputIndexPlayer;
extern bool continueLoop;
extern bool endGame;

/*
	*** НЕКОТОРЫЕ РАССУЖДЕНИЯ В ПРОЦЕССЕ РАЗРАБОТКИ ***
	1. Если мы вводим индексы с консоли
		Игровой цикл ожидает ввода в методе MakeStep
	2. Если сервер инициирует ввод индексов (по нажатию пользователем на ячейку в графическом интерфейсе)
		Игра должна продолжаться по этому событию

	Событийное программирование?


	1. Игровой цикл крутится в отдельном потоке
	2. Когда необходим ввод пользователя, поток засыпает
	3. Происходит ввод пользователя
	4. Функция обработчик будет поток игрового цикла
*/

/*
	*** ЗАДАЧИ: ***
		+ Play(continueGame) не отрабатывает уже завершенную игру
		+ убрать Undo() из Input и запоминание индексов
		+ сохранять состояния в объекте Field
			+ IGameField::operator[] -> int IGameField::At(index) + &int IGameField::At(index, indexPlayer)
			+ уметь откатываться, откатываться на определенный шаг
			+ гетер количества шагов
		- ТЕСТЫ
*/

/*
	Функция потока, отвечающего за пользовательский ввод с клавиатуры
	Резрешение ввода по condition variable
	Реализация неполная
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

// Игра, в которой пользовательские потоки "будятся" по уведомлению от игрового потока
// Пока реализация неполная
void GameThroughEvents()
{
	// std::thread user0(UserEvents, 0);
	// std::thread user1(UserEvents, 1);

	// IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>() ...

	// std::cout << "Waiting user...\n";
	// user0.join();
	// user1.join();
	// std::cout << "User completed!\n";
}


int main()
{
	return 0;
}
