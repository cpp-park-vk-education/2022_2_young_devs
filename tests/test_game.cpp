#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "compressor.h"
#include "game_loop.h"
#include "game_progress.h"

#include "config.h"

std::string operator/(std::string const &left, std::string const &right)
{
	return left + "/" + right;
}

const std::string _BASE_DIR = BASE_DIR;
const std::string _TEST_DIR = "tests";

const std::string COMMAND_CD = "cd " + (BASE_DIR / _TEST_DIR);

std::string MakeCommand(std::string const &path1, std::string const &path2)
{
	return COMMAND_CD + " && make do_diff PATH1=" + path1 + " PATH2=" + path2; 
}

const std::string _CASES_DIR 			= "input_output_test";
const std::string _ANSW_DIR 			= "correct_answers";

const std::string _TEST_CASE_1_IN 		= "1_user_vs_bot_in.txt";
const std::string _TEST_CASE_1_OUT 		= "1_user_vs_bot_out.txt";

const std::string _TEST_CASE_2_IN_0 	= "2_user_vs_user_in_0.txt";
const std::string _TEST_CASE_2_IN_1 	= "2_user_vs_user_in_1.txt";
const std::string _TEST_CASE_2_OUT 		= "2_user_vs_user_out.txt";

const std::string _TEST_CASE_3_OUT 		= "3_bot_vs_bot_out.txt";

const std::string _TEST_CASE_4_IN 		= "_user_vs_bot_pause_rollback_in.txt";
const std::string _TEST_CASE_4_OUT 		= "_user_vs_bot_pause_rollback_out.txt";


const std::string _TEST_CASE_5_IN 		= "5_user_vs_userno_logic_in.txt";
const std::string _TEST_CASE_5_OUT 		= "5_user_vs_user_no_logic_out.txt";

using namespace std::string_literals;

// Игра с примитивным ботом, который ставит O (X) в первую свободную ячейку
TEST(Test_3x3_TicTacToe, Test_GameWithSimpleBot)
{
	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();

	std::string PATH_TEST_IN 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_1_IN;
	std::string PATH_TEST_OUT 		= _BASE_DIR / _CASES_DIR / _TEST_CASE_1_OUT;
	std::string PATH_TEST_OUT_ANSW 	= _BASE_DIR / _CASES_DIR / _ANSW_DIR / _TEST_CASE_1_OUT;

	std::ifstream file_in(PATH_TEST_IN);
	std::ofstream file_out(PATH_TEST_OUT);

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

	int returnValue = system(MakeCommand(PATH_TEST_OUT, PATH_TEST_OUT_ANSW).c_str());
	EXPECT_EQ(returnValue, 0);
}

TEST(Test_3x3_TicTacToe, Test_GameTwoPlayers)
{
	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();

	std::string PATH_TEST_IN_0 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_2_IN_0;
	std::string PATH_TEST_IN_1 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_2_IN_1;
	std::string PATH_TEST_OUT 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_2_OUT;
	std::string PATH_TEST_OUT_ANSW 		= _BASE_DIR / _CASES_DIR / _ANSW_DIR / _TEST_CASE_2_OUT;

	std::ifstream file_in_user_0(PATH_TEST_IN_0);
	std::ifstream file_in_user_1(PATH_TEST_IN_1);
	std::ofstream file_out(PATH_TEST_OUT);

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

	int returnValue = system(MakeCommand(PATH_TEST_OUT, PATH_TEST_OUT_ANSW).c_str());
	EXPECT_EQ(returnValue, 0);
}

TEST(Test_3x3_TicTacToe, Test_GameTwoBots)
{
	IGameLogic *gameLogic = new TicTacToeLogic;
	IGameField *field = gameLogic->GetField();

	std::string PATH_TEST_OUT 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_3_OUT;
	std::string PATH_TEST_OUT_ANSW 		= _BASE_DIR / _CASES_DIR / _ANSW_DIR / _TEST_CASE_3_OUT;


	std::ofstream file_out(PATH_TEST_OUT);

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

	int returnValue = system(MakeCommand(PATH_TEST_OUT, PATH_TEST_OUT_ANSW).c_str());
	EXPECT_EQ(returnValue, 0);
}

std::string GamePart(size_t indexPart, std::string const &strCompressed = "")
{
	CompressorTicTacToe compressor;
	IGameLogic *gameLogic;
	IGameField *field;
	// передали сериализованное состояние игры
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
		// не передали сериализованное состояние игры
		gameLogic = new TicTacToeLogic;
		field = gameLogic->GetField();
	}

	std::string strIndexPart = std::to_string(indexPart);

	std::string PATH_TEST_IN 			= _BASE_DIR / _CASES_DIR / ("4_"s + strIndexPart + _TEST_CASE_4_IN);
	std::string PATH_TEST_OUT 			= _BASE_DIR / _CASES_DIR / ("4_"s + strIndexPart + _TEST_CASE_4_OUT);
	

	std::ofstream file_out(PATH_TEST_OUT);
	std::ifstream file_in(PATH_TEST_IN);

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

/*
	Игра с паузами и откатами
		- Команда для паузы: 10
		- Команды для отката: 11-19
			- единицы означают количество шагов для отката
			- e.g. 12 - откатить последний ход соперника и свой
*/
TEST(Test_3x3_TicTacToe, Test_GameWithPausesAndRollbacks)
{
	std::string strCompressedGameProgress = GamePart(0);
	GamePart(1, strCompressedGameProgress);

	
	std::string PATH_TEST_OUT_0 			= _BASE_DIR / _CASES_DIR / ("4_"s + std::to_string(0) + _TEST_CASE_4_OUT);
	std::string PATH_TEST_OUT_ANSW_0 		= _BASE_DIR / _CASES_DIR / _ANSW_DIR / ("4_"s + std::to_string(0) + _TEST_CASE_4_OUT);
	std::string PATH_TEST_OUT_1 			= _BASE_DIR / _CASES_DIR / ("4_"s + std::to_string(1) + _TEST_CASE_4_OUT);
	std::string PATH_TEST_OUT_ANSW_1 		= _BASE_DIR / _CASES_DIR / _ANSW_DIR / ("4_"s + std::to_string(1) + _TEST_CASE_4_OUT);

	int returnValue1 = system(MakeCommand(PATH_TEST_OUT_0, PATH_TEST_OUT_ANSW_0).c_str());
	EXPECT_EQ(returnValue1, 0);

	int returnValue2 = system(MakeCommand(PATH_TEST_OUT_1, PATH_TEST_OUT_ANSW_1).c_str());
	EXPECT_EQ(returnValue2, 0);
}

// Игра с логикой-заглушкой
TEST(Test_3x3_TicTacToe, Test_GameNoLogic)
{
	IGameLogic *gameLogic = new GameTestLogic;

	std::string PATH_TEST_IN 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_5_IN;
	std::string PATH_TEST_OUT 			= _BASE_DIR / _CASES_DIR / _TEST_CASE_5_OUT;
	std::string PATH_TEST_OUT_ANSW 		= _BASE_DIR / _CASES_DIR / _ANSW_DIR / _TEST_CASE_5_OUT;

	std::ofstream file_out(PATH_TEST_OUT);
	std::ifstream file_in_user_0_1(PATH_TEST_IN);

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

	int returnValue = system(MakeCommand(PATH_TEST_OUT, PATH_TEST_OUT_ANSW).c_str());
	EXPECT_EQ(returnValue, 0);
}
