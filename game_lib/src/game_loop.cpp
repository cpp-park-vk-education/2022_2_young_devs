#include <iostream>

#include "game_loop.h"

using namespace std::string_literals;

void IGameLoop::Play(OPTION_PLAY option)
{
	InitializeGame(option);
	size_t _currentPlayer = 0;
	while (!EndOfGame())
	{
		_currentPlayer = MakeStep(_currentPlayer);
	}
	PrintWinner();
}

TicTacToeLoop::TicTacToeLoop(IGameLogic *gameLogic, IInput *input1, IInput *input2, IOutput *output,
							 IGameProgress *progress)
	: IGameLoop(2)
{
	_input1 = (input1 == nullptr) 		? new InputStream 			: input1;
	_input2 = (input2 == nullptr) 		? new InputStream 			: input2;
	_output = (output == nullptr) 		? new OutputStreamTicTacToe : output;
	_gameLogic = (gameLogic == nullptr) ? new TicTacToeLogic(3) 	: gameLogic;
	// _progress может быть nullptr (не записываем историю игры)
	_progress = progress;
}

TicTacToeLoop::~TicTacToeLoop()
{
	// TODO
}

void TicTacToeLoop::InitializeGame(OPTION_PLAY option)
{
	isStopped = false;
	if (option = NEW_GAME)
	{
		_gameLogic->SetNewGame();
	}
}

size_t TicTacToeLoop::MakeStep(size_t indexPlayer)
{
	_output->Output(_gameLogic->GetField());
	IInput *currentInput;
	currentInput = (indexPlayer % 2 == 0) ? _input1 : _input2;
	_output->OutputMsg("> Waiting for player "s + std::to_string(indexPlayer) + "'s step"s);
	uint16_t indexCell = currentInput->InputCommand();
	// обработка команд: 10 - пауза, 11 - 19 - откат на 1 - 9 шагов соответственно
	if (indexCell == 10)
	{
		isStopped = true;
		return indexPlayer;
	}
	if (_progress && indexCell >= 11 && indexCell <= 19)
	{
		_progress->Rollback(_gameLogic->GetField(), indexCell % 10);
		_gameLogic->ReloadField();
		if (indexCell % 2 != 0)
		{
			return (indexPlayer + 1) % _countPlayers;
		}
		else
		{
			return indexPlayer;
		}
	}
	while (!_gameLogic->MakeStep(indexCell, indexPlayer, _progress))
	{
		_output->OutputMsg("> You made a mistake");
		indexCell = currentInput->InputCommand();
	}
	return (indexPlayer + 1) % _countPlayers;
	// Замечание: после неправильного ввода вторая попытка - только индекс ячейки, команду нельзя (пофиксить)
}

bool TicTacToeLoop::EndOfGame()
{
	if (isStopped)
	{
		return true;
	}
	bool isEnd = _gameLogic->EndOfGame();
	if (isEnd)
	{
		_progress->AddWinner(_currentPlayer);
		return true;
	}
	return false;
}

void TicTacToeLoop::PrintWinner()
{
	_output->Output(_gameLogic->GetField());
	ReportGame report = _gameLogic->GetReportGame();
	if (isStopped)
	{
		_output->OutputMsg("Game is stopped!");
	}
	else if (report.isDraw)
	{
		_output->OutputMsg("The game ended in a draw");
	}
	else
	{
		_output->OutputMsg("Winner is "s + (report.indexPlayer == 0 ? 'X' : 'O'));
	}
}

IGameProgress *TicTacToeLoop::GetGameProgress() const
{
	return _progress;
}
