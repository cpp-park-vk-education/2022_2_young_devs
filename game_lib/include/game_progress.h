#pragma once

#include "round_storage.h"

/*
Класс, отвечающий за ход игры
	- наследует методы класса IRoundStorage, инкапсулирующего логику хранения ходов в рамках одного раунда
	- добавляет функционал: позволяет откатываться на определенное количество шагов
*/
class IGameProgress : public IRoundStorage
{
public:
	virtual void Rollback(IGameField *field, size_t countSteps = 2) = 0;
};

class TicTacToeProgress : public IGameProgress
{
public:
	TicTacToeProgress(size_t sizeField);
	virtual size_t GetSizeField() const override
	{
		return _sizeField;
	}
	virtual void Rollback(IGameField *field, size_t countSteps = 2) override;
	virtual void AddStep(size_t indexCell, size_t indexPlayer, int16_t valueCell) override;
	virtual void AddWinner(size_t indexPlayer) override;
	virtual void InitializeField(IGameField *field) const override;
	virtual std::vector<Step> GetSteps() const override;

private:
	size_t _sizeField;
	size_t _indexWinner;
	std::vector<Step> _steps;
};