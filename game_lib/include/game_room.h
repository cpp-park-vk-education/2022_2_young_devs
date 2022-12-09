#pragma once

#include <string>
#include <vector>

#include "game_field.h"
#include "game_player.h"
#include "game_logic.h"
#include "game_output.h"


class GameRoom
{
protected:
    size_t      _room_id;
    TypeStatus  _status; 
public:
    virtual ~GameRoom() = default;
    virtual void DoAction(size_t player_id, TypeAction type, DataAction data) = 0;
    virtual Player *GetWinner() = 0;
    TypeStatus GetStatus();
    virtual std::string Serialize() = 0;
};

class T_Room : public GameRoom
{
private:
    std::vector<T_Player *> _players;
    T_GameField*    _field;
    T_GameLogic*            _logic;
    T_Output*               _output;
    std::vector<StepInfo>       _steps;
public:
    virtual void DoAction(size_t player_id, TypeAction type, DataAction data) override;
    virtual Player *GetWinner() override;
    virtual std::string Serialize() override;
    static T_Room Deserialize(std::string serialize_data);
};