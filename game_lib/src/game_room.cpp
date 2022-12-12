#include "game_room.h"
#include "helpers_func.h"

#include "reports_bug.h"

GameRoom::GameRoom()
{
    _room_id = id;
    id++;
}

TypeStatus GameRoom::GetStatus()
{
    return _status;
}

void GameRoom::Stop()
{
    if (_status == TypeStatus::Active)
    {
        _status = TypeStatus::Stopped;
    }
}


T_Room::T_Room(Player player_1, Player player_2, T_GameField *field, T_GameLogic *logic, T_Output *output, T_Bot *bot, TypeGame typeGame)
        :
        GameRoom(), _player_1(player_1), _player_2(player_2), _field(field), _logic(logic), _output(output), _bot(bot), _typeGame(typeGame)
{
    _status = TypeStatus::Active;
}

void T_Room::fillReport(ReportAction &report, Player player, TypeAction type, DataAction data)
{
    report.player       = player;
    report.typeAction   = type;
    report.data         = data;
    report.status       = _status;
    report.field        = _field;
    report.typeGame     = _typeGame;
    report.room_id      = _room_id;
    report.steps        = _steps;
};

std::tuple<Player, Player, ReportAction> T_Room::checkPlayer(Player player)
{
    ReportAction report;
    report.isValid = true;
    Player cur_player;
    Player other_player;
    if (_player_1.id == player.id)
    {
        cur_player = _player_1;
        other_player = _player_2;
    }
    else if (_player_2.id == player.id)
    {
        cur_player = _player_2;
        other_player = _player_1;
    }
    else
    {
        cur_player.id = player.id;
        report = reportCode2;
    }
    return {cur_player, other_player, report};
}

ReportAction T_Room::DoAction(Player player, TypeAction type, DataAction data)
{

    auto [cur_player, other_player, report] = checkPlayer(player);
    if (!report.isValid)
    {
        // Игрок не зарегистрирован в этой комнате
        fillReport(report, cur_player, type, data);
        _output->Output(report);
        return report;
    }



    if (_status == TypeStatus::Finished)
    {
        fillReport(report, cur_player, type, data);
        report = reportCode3;
        _output->Output(report);
        return report;
    }
    if (_status == TypeStatus::Stopped)
    {
        _status = TypeStatus::Active;
    }


    switch(type)
    {
        case TypeAction::Rollback:
        {
            // Бот не может делать откат
            if (cur_player.isBot)
            {
                fillReport(report, cur_player, type, data);
                report = reportCode7;
            }
<<<<<<< HEAD
                // 1. Откат не может быть больше количества шагов
                // 2. Нельзя откатить один ход бота, только (его ход и свой) * n
||||||| 3cfaff3
=======
            // 1. Откат не может быть больше количества шагов
            // 2. Нельзя откатить один ход бота, только (его ход и свой) * n
>>>>>>> 02cb01ac6d1f1c2a279a7b463ad66af66cff0bc8
            else if ((int)_steps.size() - data.value < 0 || data.value % 2 != 0)
            {
                fillReport(report, cur_player, type, data);
                report = reportCode8;
            }
            else if (other_player.isBot)
            {
                _field->Rollback(data.value, _steps);
                fillReport(report, cur_player, type, data);
                report.isValid = true;
            }
<<<<<<< HEAD
                // Нельзя делать откат, если играешь не с ботом
||||||| 3cfaff3
=======
            // Нельзя делать откат, если играешь не с ботом
>>>>>>> 02cb01ac6d1f1c2a279a7b463ad66af66cff0bc8
            else
            {
                fillReport(report, cur_player, type, data);
                report = reportCode6;
            }
            break;
        }

        case TypeAction::Step:
        {
            TypeCell cell = cur_player.cell;
            // Ходит другой человек
            if (!_steps.empty() && _steps.back().cell == cell)
            {
                fillReport(report, cur_player, type, data);
                report = reportCode5;
                _output->Output(report);
                return report;
            }
            fillReport(report, cur_player, type, data);
            report.data.value = data.value;
            ReportAction report_after_step;
            if (cur_player.isBot)
            {
                report_after_step = _bot->MakeStep(report);
            }
            else
            {
                report_after_step = _logic->MakeStep(report);
            }
            report = report_after_step;
            // Если ход правильный, добавляем в вектор ходов
            if (report.isValid)
            {
                addStep(cur_player.id, report.data.value, cell);
            }
            // Проверка на конец игры
            if (report.result.isEnd)
            {
                report.result.winner = cur_player;
                result = report.result;
                _status = TypeStatus::Finished;
            }
            fillReport(report, cur_player, type, data);
            break;
        }
    }
    _output->Output(report);
    return report;
}

void T_Room::addStep(size_t player_id, size_t index, TypeCell cell)
{
    StepInfo step = {
            .player_id = player_id,
            .index = index,
            .cell = cell
    };
    _steps.push_back(step);
}

GameResult T_Room::GetResult()
{
    if (result)
    {
        return *result;
    }
    return { .isEnd = false };
}

std::vector<Player> T_Room::GetPlayers()
{
    return { _player_1, _player_2 };
}