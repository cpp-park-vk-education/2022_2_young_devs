#include "game_room.h"
#include "helpers_func.h"

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

ReportAction T_Room::DoAction(Player player, TypeAction type, DataAction data)
{
    ReportAction report;

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
        fillReport(report, cur_player, type, data);
        report.isValid = false;
        report.error.codeError = 4;
        report.error.messageError = "The player is not registered in the game room";
        _output->Output(report);  
        return report;
    }

    if (_status == TypeStatus::Finished)
    {
        fillReport(report, cur_player, type, data);
        report.isValid = false;
        report.error.codeError = 3;
        report.error.messageError = "The game is already over";
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
            if (cur_player.isBot)
            {
                fillReport(report, cur_player, type, data);
                report.isValid = false;
                report.error.codeError = 7;
                report.error.messageError = "Bot can't rollback";
            }
            else if ((int)_steps.size() - data.value < 0 || data.value % 2 != 0)
            {
                fillReport(report, cur_player, type, data);
                report.isValid = false;
                report.error.codeError = 8;
                report.error.messageError = "Rollback is impossible.";
            }
            else if (other_player.isBot)
            {
                _field->Rollback(data.value, _steps);
                fillReport(report, cur_player, type, data);
                report.isValid = true;
            }
            else
            {
                fillReport(report, cur_player, type, data);
                report.isValid = false;
                report.error.codeError = 6;
                report.error.messageError = "Rollback is only available with a bot";
            }
            break;
        }

        case TypeAction::Step:
        {
            TypeCell cell = cur_player.cell;
            if (!_steps.empty() && _steps.back().cell == cell)
            {
                fillReport(report, cur_player, type, data);
                report.isValid = false;
                report.error.codeError = 5;
                report.error.messageError = "The other player must move";
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
            // заполняет isValid и result 
            if (report.isValid)
            {
                addStep(cur_player.id, report.data.value, cell);
            }
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

std::string T_Room::Serialize()
{
    puts("s");
    return {};
}

T_Room T_Room::Deserialize(std::string serialize_data)
{
    puts("1");
    return {};
}

std::vector<Player> T_Room::GetPlayers()
{
    return { _player_1, _player_2 };
}