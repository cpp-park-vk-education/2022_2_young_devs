#include "input_output.h"
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

std::mutex mutex;
std::condition_variable cv;

ssize_t indexGlobal = -1;
ssize_t needsInputIndexPlayer = -1;
bool continueLoop = false;
bool endGame = false;

uint16_t InputConsole::InputIndex()
{
	std::cout << "Enter index of cell\n";
	uint16_t indexCell;
	std::cin >> indexCell;
	_seqIndices.push_back(indexCell);
	return indexCell;
}
void InputConsole::Undo()
{
	if (!_seqIndices.empty())
	{
		_seqIndices.pop_back();
	}
}

// ---------------------------------------------------------

uint16_t InputEvent::InputIndex()
{
	{
		std::unique_lock<std::mutex> lock(mutex);
		needsInputIndexPlayer = _indexPlayer;
	}
	cv.notify_all();
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (!continueLoop && indexGlobal == -1)
		{
			cv.wait(lock);
		}
	}
	continueLoop = false;
	uint16_t indexLocal = indexGlobal;
	indexGlobal = -1;
	_seqIndices.push_back(indexLocal);
	return indexLocal;
}
void InputEvent::Undo()
{
	if (!_seqIndices.empty())
	{
		_seqIndices.pop_back();
	}
}

// ------------------------------------------------------------

uint16_t InputBot::InputIndex()
{
	std::cout << "Bot's step...\n";
	for (size_t i = 0; i < _field->Size() * _field->Size(); ++i)
	{
		if ((*_field)[i] == E)
		{
			_seqIndices.push_back(i);
			return i;
		}
	}
	// return ?
}
void InputBot::Undo()
{
	_seqIndices.pop_back();
}

// ------------------------------------------------------------

void OutputConsole::Output(IGameField *field) const
{
	for (size_t i = 0; i < field->Size() * field->Size(); ++i)
	{
		std::cout << std::setw(2) << (*field)[i] << ' ';
		if (i % field->Size() == field->Size() - 1)
		{
			std::cout << '\n';
		}
	}
}
void OutputConsole::OutputMsg(std::string const &message) const
{
	std::cout << message << '\n';
}
