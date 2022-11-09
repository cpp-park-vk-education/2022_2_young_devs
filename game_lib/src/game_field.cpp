#include "game_field.h"
#include <cassert>

TicTacToeField::TicTacToeField(size_t n) : _n(n), _field(n * n, E)
{
}
int16_t TicTacToeField::operator[](size_t index) const
{
    assert(index < _field.size());
    return _field[index];
}
int16_t &TicTacToeField::operator[](size_t index)
{
    assert(index < _field.size());
    return _field[index];
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
// копирует данные, указателем не владеет
void TicTacToeField::SetField(IGameField *field)
{
    std::copy(field->Begin(), field->End(), _field.begin());
}

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

int16_t GameTestField::operator[](size_t index) const
{
    return _testField[index];
}

int16_t &GameTestField::operator[](size_t index)
{
    return _testField[index];
}

void GameTestField::Clear()
{
    std::fill(_testField.begin(), _testField.end(), 7);
}
// копирует данные, указателем не владеет
void GameTestField::SetField(IGameField *field)
{
    std::copy(field->Begin(), field->End(), _testField.begin());
}
