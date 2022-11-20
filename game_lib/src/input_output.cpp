#include "input_output.h"
#include <condition_variable>
#include <iomanip>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unistd.h>

std::mutex mutex;
std::condition_variable cv;

ssize_t indexGlobal = -1;
ssize_t needsInputIndexPlayer = -1;
bool continueLoop = false;
bool endGame = false;

// ************ InputStream ************

InputStream::InputStream(std::istream &in) : _in(in)
{
}

uint16_t InputStream::InputCommand()
{
	uint16_t indexCell;
	_in >> indexCell;
	return indexCell;
}

// ************ InputEvent ************

uint16_t InputEvent::InputCommand()
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
	return indexLocal;
}

// ************ InputBot ************

uint16_t InputBot::InputCommand()
{
	sleep(1);
	for (size_t i = 0; i < _field->Size() * _field->Size(); ++i)
	{
		if (_field->At(i) == E)
		{
			return i;
		}
	}
	// return ?
}

// ************ OutputStreamTicTacToe ************

OutputStreamTicTacToe::OutputStreamTicTacToe(std::ostream &out) : _out(out)
{
}

void OutputStreamTicTacToe::Output(IGameField *field) const
{
	for (size_t i = 0; i < field->Size() * field->Size(); ++i)
	{
		std::string strCell;
#define PRINT(a) strCell = #a;
		switch (field->At(i))
		{
		case X: {
			PRINT(X);
			break;
		}
		case O: {
			PRINT(O);
			break;
		}
		case E: {
			strCell = "-";
			break;
		}
		default: {
			strCell = "~";
		}
		}
		_out << std::setw(2) << strCell << ' ';
		if (i % field->Size() == field->Size() - 1)
		{
			_out << '\n';
		}
	}
}
void OutputStreamTicTacToe::OutputMsg(std::string const &message) const
{
	_out << message << '\n';
}
