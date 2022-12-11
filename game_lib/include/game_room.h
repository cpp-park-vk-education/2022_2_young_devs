#pragma once

#include <string>
#include <vector>
#include <optional>

#include "game_field.h"
#include "game_logic.h"
#include "game_output.h"
#include "bot.h"

class GameRoom
{
private:
    inline static size_t id = 0;
protected:
    size_t      _room_id;
    TypeStatus  _status; 
public:
    GameRoom();
    virtual ~GameRoom() = default;
    virtual ReportAction DoAction(Player player, TypeAction type, DataAction data = {}) = 0;
    virtual GameResult GetResult() = 0;
    virtual std::vector<Player> GetPlayers() = 0;
    TypeStatus GetStatus();
    void Stop();
    virtual std::string Serialize() = 0;

};

class T_Room : public GameRoom
{
private:
    std::optional<GameResult> result = std::nullopt;
    TypeGame _typeGame;
    Player _player_1;
    Player _player_2;
    T_GameField*    _field;
    T_GameLogic*    _logic;
    T_Bot*          _bot;
    T_Output*       _output;
    std::vector<StepInfo>  _steps;
    void fillReport(ReportAction &report, Player player, TypeAction type, DataAction data);
    void addStep(size_t player_id, size_t index, TypeCell cell);
public:
    T_Room() = default;
    T_Room(Player player_1, Player player_2, T_GameField *field, T_GameLogic *logic, T_Output *output, T_Bot *bot, TypeGame typeGame = TypeGame::OT);
    virtual ReportAction DoAction(Player player, TypeAction type, DataAction data = {}) override;
    virtual std::vector<Player> GetPlayers() override;
    virtual GameResult GetResult() override;
    virtual std::string Serialize() override;
    static T_Room Deserialize(std::string serialize_data);
};
