#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "compressor.h"
#include "game_loop.h"
#include "game_progress.h"

extern std::mutex mutex;
extern std::condition_variable cv;

extern ssize_t indexGlobal;
extern ssize_t needsInputIndexPlayer;
extern bool continueLoop;
extern bool endGame;

using namespace std::string_literals;

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

// ************************************
// 1. Игра с примитивным ботом, который ставит O (X) в первую свободную ячейку
void GameWithSimpleBot()
{
	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();

	std::ofstream file_out("../input_output_data/1_user_vs_bot_out.txt");
	std::ifstream file_in("../input_output_data/1_user_vs_bot_in.txt");

	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
							   .withGameLogic(gameLogic)
							   .withInputPlayerOne(new InputStream(file_in))
							   .withInputPlayerTwo(new InputBot(1, field))
							   .withOutput(new OutputStreamTicTacToe(file_out))
							   .withProgress(new TicTacToeProgress(3))
							   .build();
	ticTacToe->Play();
	file_out.close();
	file_in.close();

	delete gameLogic;
}

// ************************************
// 2.
void GameTwoPlayers()
{
	IGameLogic *gameLogic = new TicTacToeLogic;

	std::ofstream file_out("../input_output_data/2_user_vs_user_out.txt");
	std::ifstream file_in_user_0("../input_output_data/2_user_vs_user_in_0.txt");
	std::ifstream file_in_user_1("../input_output_data/2_user_vs_user_in_1.txt");

	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
							   .withGameLogic(gameLogic)
							   .withInputPlayerOne(new InputStream(file_in_user_0))
							   .withInputPlayerTwo(new InputStream(file_in_user_1))
							   .withOutput(new OutputStreamTicTacToe(file_out))
							   .withProgress(new TicTacToeProgress(3))
							   .build();
	ticTacToe->Play();
	file_out.close();
	file_in_user_0.close();
	file_in_user_1.close();

	delete gameLogic;
}

// ************************************
// 3.
void GameTwoBots()
{
	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();

	std::ofstream file_out("../input_output_data/3_bot_vs_bot_out.txt");

	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
							   .withGameLogic(gameLogic)
							   .withInputPlayerOne(new InputBot(0, field))
							   .withInputPlayerTwo(new InputBot(1, field))
							   .withOutput(new OutputStreamTicTacToe(file_out))
							   .withProgress(new TicTacToeProgress(3))
							   .build();

	ticTacToe->Play();
	file_out.close();

	delete gameLogic;
}

std::string GamePart(size_t indexPart, std::string const &strCompressed = "")
{
	CompressorTicTacToe compressor;
	IGameLogic *gameLogic;
	IGameField *field;
	if (!strCompressed.empty())
	{
		IRoundStorage *roundStorage = compressor.DecompressRound(strCompressed);
		field = new TicTacToeField(3);
		roundStorage->InitializeField(field);
		gameLogic = new TicTacToeLogic(field->Size(), field);
		delete roundStorage;
	}
	else
	{
		gameLogic = new TicTacToeLogic;
		field = gameLogic->GetField();
	}

	std::string strIndexPart = std::to_string(indexPart);
	std::ofstream file_out("../input_output_data/4_"s + strIndexPart + "_user_vs_bot_pause_rollback_out.txt"s);
	std::ifstream file_in("../input_output_data/4_"s + strIndexPart + "_user_vs_bot_pause_rollback_in.txt"s);

	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
							   .withGameLogic(gameLogic)
							   .withInputPlayerOne(new InputStream(file_in))
							   .withInputPlayerTwo(new InputBot(1, field))
							   .withOutput(new OutputStreamTicTacToe(file_out))
							   .withProgress(new TicTacToeProgress(3))
							   .build();
	ticTacToe->Play(CONTINUE);

	file_out.close();
	file_in.close();

	IGameProgress *progress = ticTacToe->GetGameProgress();

	// CompressRound принимает IRoundStorage *
	std::string strNewCompressed = compressor.CompressRound(progress);

	delete gameLogic;

	return strNewCompressed;
}

// ************************************
/*
	4. Игра с паузами и откатами
		- Команда для паузы: 10
		- Команды для отката: 11-19
			- единицы означают количество шагов для отката
			- e.g. 12 - откатить последний ход соперника и свой
*/
void GameWithPausesAndRollbacks()
{
	std::string strCompressedGameProgress = GamePart(0);
	GamePart(1, strCompressedGameProgress);
}

// ************************************
// 5. Игра без логики
void GameNoLogic()
{
	IGameLogic *gameLogic = new GameTestLogic;

	std::ofstream file_out("../input_output_data/5_user_vs_user_no_logic_out.txt");
	std::ifstream file_in_user_0_1("../input_output_data/5_user_vs_userno_logic_in.txt");

	IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
							   .withGameLogic(gameLogic)
							   .withInputPlayerOne(new InputStream(file_in_user_0_1))
							   .withInputPlayerTwo(new InputStream(file_in_user_0_1))
							   .withOutput(new OutputStreamTicTacToe(file_out))
							   .withProgress(new TicTacToeProgress(3))
							   .build();
	ticTacToe->Play();
	file_out.close();
	file_in_user_0_1.close();

	delete gameLogic;
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

void Go()
{
	// Выбор теста

	GameTwoPlayers();
	GameWithSimpleBot();
	GameTwoBots();
	GameWithPausesAndRollbacks();
	GameNoLogic();
}

int main()
{
	Go();
	return 0;
}
