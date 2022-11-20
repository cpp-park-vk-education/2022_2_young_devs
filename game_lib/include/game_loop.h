#pragma once

#include "game_logic.h"
#include "game_progress.h"
#include "input_output.h"
#include <cstdint>

enum OPTION_PLAY
{
	NEW_GAME = 0,
	CONTINUE
};

// Паттерн "Шаблонный метод"
class IGameLoop
{
protected:
	size_t _currentPlayer = 0;
	size_t _countPlayers;
	virtual void InitializeGame(OPTION_PLAY option) = 0;
	virtual size_t MakeStep(size_t indexPlayer) = 0;
	virtual bool EndOfGame() = 0;
	virtual void PrintWinner() = 0;
	virtual ~IGameLoop() = default;

public:
	IGameLoop(size_t countPlayers) : _countPlayers(countPlayers)
	{
	}
	virtual IGameProgress *GetGameProgress() const = 0;
	void Play(OPTION_PLAY option = NEW_GAME);
};

class TicTacToeLoop : public IGameLoop
{
public:
	TicTacToeLoop(IGameLogic *gameLogic = nullptr, IInput *input1 = nullptr, IInput *input2 = nullptr,
				  IOutput *output = nullptr, IGameProgress *progress = nullptr);
	virtual ~TicTacToeLoop();
	virtual IGameProgress *GetGameProgress() const override;

protected:
	virtual void InitializeGame(OPTION_PLAY option) override;
	// возвращает индекс следующего игрока
	virtual size_t MakeStep(size_t indexPlayer) override;
	virtual bool EndOfGame() override;
	virtual void PrintWinner() override;

private:
	// разные интерфейсы одного объекта
	bool isStopped = false;
	IGameLogic *_gameLogic;
	IInput *_input1;
	IInput *_input2;
	IOutput *_output;
	IGameProgress *_progress;
};

// Конструирует объект игрового цикла (IGameLoop *)
template <typename TypeGameLoop> class GameLoopBuilder
{
public:
	IGameLoop *build()
	{
		return new TypeGameLoop(_gameLogic, _input1, _input2, _output, _progress);
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

	GameLoopBuilder &withProgress(IGameProgress *progress)
	{
		_progress = progress;
		return *this;
	}

private:
	IGameLogic *_gameLogic = nullptr;
	IInput *_input1 = nullptr;
	IInput *_input2 = nullptr;
	IOutput *_output = nullptr;
	IGameProgress *_progress = nullptr;
};
