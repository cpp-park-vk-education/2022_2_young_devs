#include "game_logic.h"
#include <algorithm>
#include <iostream>

// ************ TicTacToeLogic ************ 

TicTacToeLogic::TicTacToeLogic(size_t n, IGameField *field) : _n(n), _sums(n * 2 + 2, 0)
{
	if (field == nullptr)
	{
		_field = new TicTacToeField(_n);
	}
	else
	{
		_field = field;
		_n = field->Size();
		// пересчет состояния игры с учетом переданного поля
		ReloadField();
	}
}

void TicTacToeLogic::SetNewGame()
{
	_field->Clear();
	_winner = E;
	std::fill(_sums.begin(), _sums.end(), 0);
}
bool TicTacToeLogic::MakeStep(uint16_t indexCell, size_t indexPlayer, IGameProgress *progress)
{
	CELL cell = indexPlayer % 2 == 0 ? X : O;
	if (indexCell >= (_n * _n) || _field->At(indexCell) != E)
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		return false;
	}
	_field->ChangeField(indexCell, (int16_t)cell);
	// итеративный пересчет состояния игры после каждого хода
	CommitStep(indexCell, cell);
	if (progress)
	{
		// запись хода в историю ходов
		progress->AddStep(indexCell, indexPlayer, (int16_t)cell);
	}
	return true;
}
bool TicTacToeLogic::EndOfGame()
{
	for (size_t i = 0; i < _sums.size(); ++i)
	{
		if (_sums[i] == _n)
		{
			_winner = X;
			return true;
		}
		if (_sums[i] == -_n)
		{
			_winner = O;
			return true;
		}
	}
	auto it = std::find(_field->Begin(), _field->End(), E);
	if (it == _field->End())
	{
		_winner = E;
		return true;
	}
	return false;
}
ReportGame TicTacToeLogic::GetReportGame() const
{
	ReportGame report;
	if (_winner == E)
	{
		report.isDraw = true;
	}
	else
	{
		report.isDraw = false;
		report.indexPlayer = _winner == X ? 0 : 1;
	}
	return report;
}
IGameField *TicTacToeLogic::GetField() const
{
	return _field;
}

// пересчет состояния игры после хода
void TicTacToeLogic::CommitStep(uint16_t indexCell, CELL cell)
{
	_sums[indexCell / _n] += cell;
	_sums[indexCell % _n + _n] += cell;
	if (indexCell % _n == indexCell / _n)
	{
		_sums[_n * 2] += cell;
	}
	if (indexCell / _n == (_n - 1) - indexCell % _n)
	{
		_sums[_n * 2 + 1] += cell;
	}
}

void TicTacToeLogic::ReloadField()
{
	_sums.assign(_field->Size() * 2 + 2, E);
	for (size_t i = 0; i < _field->Size() * _field->Size(); ++i)
	{
		CommitStep(i, (CELL)_field->At(i));
	}
}

// ************ GameTestLogic ************ 

GameTestLogic::GameTestLogic(size_t n, IGameField *field)
{
	_field = (field == nullptr) ? new GameTestField(n) : field;
}
void GameTestLogic::SetNewGame()
{
	_field->Clear();
}
bool GameTestLogic::MakeStep(uint16_t indexCell, size_t indexPlayer, IGameProgress *progress)
{
	(void)indexCell;
	(void)indexPlayer;
	(void)progress;
	return true;
}
bool GameTestLogic::EndOfGame()
{
	static size_t cnt = 0;
	if (cnt == 5)
	{
		return true;
	}
	cnt++;
	return false;
}
ReportGame GameTestLogic::GetReportGame() const
{
	ReportGame report = {.isDraw = false, .indexPlayer = 1};
	return report;
}

IGameField *GameTestLogic::GetField() const
{
	return _field;
}

void GameTestLogic::ReloadField()
{
}
