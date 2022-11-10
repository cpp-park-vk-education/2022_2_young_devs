#include "game_loop.h"

void IGameLoop::Play(bool isNewGame)
{
	InitializeGame(isNewGame);
	size_t indexPlayer = 0;
	while (!EndOfGame())
	{
		MakeStep(indexPlayer);
		indexPlayer = (indexPlayer + 1) % _countPlayers;
	}
	PrintWinner();
}

TicTacToeLoop::TicTacToeLoop(IGameLogic *gameLogic, IInput *input1, IInput *input2, IOutput *output) : IGameLoop(2)
{
	_input1 = (input1 == nullptr) ? new InputConsole : input1;
	_input2 = (input2 == nullptr) ? new InputConsole : input2;
	_output = (output == nullptr) ? new OutputConsole : output;
	_gameLogic = (gameLogic == nullptr) ? new TicTacToeLogic(3) : gameLogic;
}

TicTacToeLoop::~TicTacToeLoop()
{
	// TODO
}

void TicTacToeLoop::InitializeGame(bool isNewGame)
{
	stopGame = false;
	_gameLogic->SetNewGame(isNewGame);
}

void TicTacToeLoop::MakeStep(size_t indexPlayer)
{
	_output->Output(_gameLogic->GetField());
	IInput *currentInput;
	currentInput = (indexPlayer % 2 == 0) ? _input1 : _input2;
	uint16_t indexCell = currentInput->InputIndex();
	if (indexCell == 911)
	{
		stopGame = true;
		return;
	}
	while (!_gameLogic->MakeStep(indexCell, indexPlayer))
	{
		currentInput->Undo();
		indexCell = currentInput->InputIndex();
	}
}

bool TicTacToeLoop::EndOfGame()
{
	if (stopGame)
	{
		return true;
	}
	return _gameLogic->EndOfGame();
}

void TicTacToeLoop::PrintWinner()
{
	_output->Output(_gameLogic->GetField());
	ReportGame report = _gameLogic->GetReportGame();
	if (report.isDraw)
	{
		_output->OutputMsg("The game ended in a draw\n");
	}
	else
	{
		_output->OutputMsg(std::string("Winner is ") + (report.indexPlayer == 0 ? "X\n" : "O\n"));
	}
}
