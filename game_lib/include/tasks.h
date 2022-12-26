#pragma once

#include "game_room.h"

class Task
{
protected:
    GameRoom *_room;
public:
    Task(GameRoom *room);
    virtual void operator()() = 0;
};


class T_StepTask : public Task
{
private:
    Player _player;
    size_t _index;
public:
    T_StepTask(GameRoom *room, Player player, size_t index);
    virtual void operator()() override;
};

class T_RollbackTask : public Task
{
private:
    Player _player;
    size_t _stepsCount;
public:
    T_RollbackTask(GameRoom *room, Player player, size_t stepsCount);
    virtual void operator()() override;
};

