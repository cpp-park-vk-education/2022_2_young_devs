#include "game_room.h"

TypeStatus GameRoom::GetStatus()
{
    return _status;
}

void T_Room::DoAction(size_t player_id, TypeAction type, DataAction data)
{
    ReportAction report;
    switch(type)
    {
        case TypeAction::Rollback:
        {
            _field->Rollback(data.value, _steps);
            ReportAction report1 = {
                .player_id = player_id,
                .data = data,
                .typeAction = TypeAction::Rollback, 
                .status = _status,
                // .typeGame = ....
                .isValid = true, // хотя в Rollback можно передать неверное значение
                .field = _field
            };
            report = report1;
            break;
        }

        case TypeAction::Step:
        {
            TypeCell cell;
            if (player_id == _players[0]->Id())
            {
                cell = TypeCell::X;
            }
            else
            {
                cell = TypeCell::O;
            }
            report = _logic->MakeStep(cell, data.value, _field);
            report.player_id = player_id;
            report.data = data;
            report.status = _status;
            report.typeAction = TypeAction::Step;
            report.field = _field;
            break;
        }

        case TypeAction::Stop:
        {
            _status = TypeStatus::Stopped;
            ReportAction report1 = {
                .player_id = player_id,
                .data = data,
                .typeAction = TypeAction::Stop, 
                .status = _status,
                // .typeGame = ....
                .isValid = true, 
                .field = _field 
            };
            report = report1;
            break;
        }
    }
    _output->Output(report);
}

Player *T_Room::GetWinner()
{
    
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