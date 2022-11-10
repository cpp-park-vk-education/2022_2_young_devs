#pragma once

#include "game_field.h"
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
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) = 0;
	virtual bool EndOfGame() = 0;
	virtual ReportGame GetReportGame() const = 0;
	virtual void SetNewGame(bool isNewGame = true) = 0;
	virtual IGameField *GetField() const = 0;
};

class TicTacToeLogic : public IGameLogic
{
public:
	TicTacToeLogic(size_t n = 3, IGameField *field = nullptr);
	// копирует данные, указателем не владеет
	// virtual void setState(IGameField *field = nullptr) override
	virtual void SetNewGame(bool isNewGame = true) override;
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) override;
	virtual bool EndOfGame() override;
	virtual ReportGame GetReportGame() const override;
	virtual IGameField *GetField() const override;

private:
	size_t _n;
	IGameField *_field;
	std::vector<int8_t> _sums;
	CELL _winner = E;
	void CommitState(uint16_t indexCell, CELL cell);
};

class GameTestLogic : public IGameLogic
{
public:
	GameTestLogic(size_t n = 3, IGameField *field = nullptr);
	virtual void SetNewGame(bool isNewGame = true) override;
	virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) override;
	virtual bool EndOfGame() override;
	virtual ReportGame GetReportGame() const override;
	virtual IGameField *GetField() const override;

private:
	IGameField *_field;
};
