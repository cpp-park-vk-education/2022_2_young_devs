#pragma once

#include "game_field.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class IInput
{
public:
	virtual uint16_t InputCommand() = 0;
};

class IOutput
{
public:
	virtual void Output(IGameField *field) const = 0;
	virtual void OutputMsg(std::string const &message) const = 0;
};

// ввод команд с istream
// команда: индекс ячейки, команда завершения игры, команда отката ходов
class InputStream : public IInput
{
private:
	std::istream &_in;

public:
	InputStream(std::istream &in = std::cin);
	virtual uint16_t InputCommand() override;
};

// инициирование события ввода команд в пользовательских потоках
class InputEvent : public IInput
{
private:
	size_t _indexPlayer;

public:
	InputEvent(size_t indexPlayer) : _indexPlayer(indexPlayer)
	{
	}
	virtual uint16_t InputCommand() override;
};

// Бот. Хранит ссылку на игровое поле для анализа хода игры.
class InputBot : public IInput
{
private:
	IGameField *_field;
	size_t _indexPlayer;

public:
	InputBot(size_t indexPlayer, IGameField *field) : _field(field), _indexPlayer(indexPlayer)
	{
	}
	virtual uint16_t InputCommand() override;
};

// Вывод в ostream
class OutputStreamTicTacToe : public IOutput
{
public:
	OutputStreamTicTacToe(std::ostream &out = std::cout);
	virtual void Output(IGameField *field) const override;
	virtual void OutputMsg(std::string const &message) const override;

private:
	std::ostream &_out;
};
