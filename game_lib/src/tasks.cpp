#include "tasks.h"
#include "game_logic.h"

#include "cond_var.h"


Task::Task(GameRoom *room) : _room(room), room_id(room->id)
{
}

T_StepTask::T_StepTask(GameRoom *room, Player player, size_t index) 
    : 
    Task(room), 
    _player(player),
    _index(index)
{
}

void T_StepTask::operator()()
{
    ReportAction report = _room->DoAction(_player, TypeAction::Step, { .value = _index });
    if (!report.isValid)
    {
        return;
    }
    if (!report.result.isEnd)
    {
        std::vector<Player> players = _room->GetPlayers();
        // ход бота, если игра с ботом
        if (players[0].id == _player.id && players[1].isBot)
        {
            _room->DoAction(players[1], TypeAction::Step);
        }
        if (players[1].id == _player.id && players[0].isBot)
        {
            _room->DoAction(players[0], TypeAction::Step);
        }
    }
}

T_RollbackTask::T_RollbackTask(GameRoom *room, Player player, size_t stepsCount) 
    : 
    Task(room),
    _player(player),
    _stepsCount(stepsCount)
{
}

void T_RollbackTask::operator()()
{
    _room->DoAction(_player, TypeAction::Rollback, { .value = _stepsCount });
}