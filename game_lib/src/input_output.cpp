#include "input_output.h"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;
std::condition_variable cv;

ssize_t indexGlobal = -1;
bool needsInput = false;
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

uint16_t InputEvent::InputIndex()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        needsInput = true;
    }
    cv.notify_one();
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

void OutputConsole::Output(IGameField *field) const
{
    for (size_t i = 0; i < field->Size() * field->Size(); ++i)
    {
        std::cout << (*field)[i] << ' ';
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
