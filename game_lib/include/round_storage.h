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

class IRoundStorage
{
public:
    virtual void AddStep(size_t indexCell, size_t indexPlayer, int16_t valueCell) = 0;
	virtual void AddWinner(size_t indexPlayer) = 0;
	virtual void InitializeField(IGameField *field) const = 0;
	virtual std::vector<Step> GetSteps() const = 0;
    virtual size_t GetSizeField() const = 0;
};