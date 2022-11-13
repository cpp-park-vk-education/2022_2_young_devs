#pragma once

#include "game_field.h"
#include "game_progress.h"
#include <functional>
#include <sys/types.h>
#include <vector>

struct ReportGame
{
	bool isDraw = false;
	ssize_t indexPlayer = -1;
};

class IGameLogic
{
public:
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer, IGameProgress *progress = nullptr) = 0;
	virtual bool EndOfGame() = 0;
	virtual ReportGame GetReportGame() const = 0;
	virtual void SetNewGame() = 0;
	virtual IGameField *GetField() const = 0;
	virtual void ReloadField() = 0;
};

class TicTacToeLogic : public IGameLogic
{
public:
	TicTacToeLogic(size_t n = 3, IGameField *field = nullptr);
	virtual void SetNewGame() override;
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer, IGameProgress *progress = nullptr) override;
	virtual bool EndOfGame() override;
	virtual ReportGame GetReportGame() const override;
	virtual IGameField *GetField() const override;
	// пересчет состояния игры с учетом текущего поля
	// т.к. поле могут изменить извне (GameProgress откатывает состояние, изменяя игровое поле)
	virtual void ReloadField() override;

private:
	void CommitStep(uint16_t indexCell, CELL cell);

	size_t _n;
	IGameField *_field;
	std::vector<int8_t> _sums;
	CELL _winner = E;
};

// Логика-заглушка
class GameTestLogic : public IGameLogic
{
public:
	GameTestLogic(size_t n = 3, IGameField *field = nullptr);
	virtual void SetNewGame() override;
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer, IGameProgress *progress = nullptr) override;
	virtual bool EndOfGame() override;
	virtual ReportGame GetReportGame() const override;
	virtual IGameField *GetField() const override;
	virtual void ReloadField() override;

private:
	IGameField *_field;
};
