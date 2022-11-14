#include "game_field.h"
#include <cassert>
#include <iostream>

// ************ TicTacToeField ************

TicTacToeField::TicTacToeField(size_t n) : _n(n), _field(n * n, E)
{
}
int16_t TicTacToeField::At(size_t index) const
{
	assert(index < _field.size());
	return _field[index];
}

void TicTacToeField::ChangeField(size_t indexCell, int16_t valueCell)
{
	assert(indexCell < _field.size());
	_field[indexCell] = valueCell;
}

size_t TicTacToeField::Size() const
{
	return _n;
}
typename std::vector<int16_t>::iterator TicTacToeField::Begin()
{
	return _field.begin();
}
typename std::vector<int16_t>::iterator TicTacToeField::End()
{
	return _field.end();
}
void TicTacToeField::Clear()
{
	std::fill(_field.begin(), _field.end(), E);
}
void TicTacToeField::SetField(IGameField *field)
{
	std::copy(field->Begin(), field->End(), _field.begin());
}

// ************ GameTestField ************

GameTestField::GameTestField(size_t n) : _n(n), _testField(n * n, 0)
{
	std::fill(_testField.begin(), _testField.end(), 7);
}
typename std::vector<int16_t>::iterator GameTestField::Begin()
{
	return _testField.begin();
}

typename std::vector<int16_t>::iterator GameTestField::End()
{
	return _testField.end();
}

size_t GameTestField::Size() const
{
	return 3;
}

int16_t GameTestField::At(size_t index) const
{
	return _testField[index];
}

void GameTestField::ChangeField(size_t indexCell, int16_t valueCell)
{
	_testField[indexCell] = valueCell;
}

void GameTestField::Clear()
{
	std::fill(_testField.begin(), _testField.end(), 7);
}

void GameTestField::SetField(IGameField *field)
{
	std::copy(field->Begin(), field->End(), _testField.begin());
}
