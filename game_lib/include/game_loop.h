#pragma once

#include "game_logic.h"
#include "input_output.h"
#include <cstdint>

class IGameLoop
{
protected:
	size_t _countPlayers;
	virtual void InitializeGame(bool isNewGame = true) = 0;
	virtual void MakeStep(size_t indexPlayer) = 0;
	virtual bool EndOfGame() = 0;
	virtual void PrintWinner() = 0;
	virtual ~IGameLoop() = default;

public:
	IGameLoop(size_t countPlayers) : _countPlayers(countPlayers)
	{
	}
	void Play(bool isNewGame = true);
};

class TicTacToeLoop : public IGameLoop
{
public:
	TicTacToeLoop(IGameLogic *gameLogic = nullptr, IInput *input1 = nullptr, IInput *input2 = nullptr,
				  IOutput *output = nullptr);
	virtual ~TicTacToeLoop();

protected:
	virtual void InitializeGame(bool isNewGame = true) override;
	virtual void MakeStep(size_t indexPlayer) override;
	virtual bool EndOfGame() override;
	virtual void PrintWinner() override;

private:
	// разные интерфейсы одного объекта
	bool stopGame = false;
	IGameLogic *_gameLogic;
	IInput *_input1;
	IInput *_input2;
	IOutput *_output;
};

template <typename TypeGameLoop> class GameLoopBuilder
{
public:
	IGameLoop *build()
	{
		return new TypeGameLoop(_gameLogic, _input1, _input2, _output);
	}

	GameLoopBuilder &withGameLogic(IGameLogic *gameLogic)
	{
		_gameLogic = gameLogic;
		return *this;
	}

	GameLoopBuilder &withInputPlayerOne(IInput *input)
	{
		_input1 = input;
		return *this;
	}

	GameLoopBuilder &withInputPlayerTwo(IInput *input)
	{
		_input2 = input;
		return *this;
	}

	GameLoopBuilder &withOutput(IOutput *output)
	{
		_output = output;
		return *this;
	}

private:
	IGameLogic *_gameLogic = nullptr;
	IInput *_input1 = nullptr;
	IInput *_input2 = nullptr;
	IOutput *_output = nullptr;
};
