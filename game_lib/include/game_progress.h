#pragma once

#include "game_field.h"

struct Step
{
	size_t indexCell;
	size_t indexPlayer;
	int16_t valueCell;
};

/*
	Хранение ходов игроков во время игры: вектор структур Step
*/
class IGameProgress
{
public:
	// возвращает измененное поле
	virtual size_t GetSizeField() const = 0;
	virtual void Rollback(IGameField *field, size_t countSteps = 2) = 0;
	virtual void AddStep(size_t indexCell, size_t indexPlayer, int16_t valueCell) = 0;
	virtual void AddWinner(size_t indexPlayer) = 0;
	virtual void InitializeField(IGameField *field) const = 0;
	virtual std::vector<Step> GetSteps() const = 0;
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