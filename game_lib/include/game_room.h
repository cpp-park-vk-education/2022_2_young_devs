#pragma once

#include <string>
#include <vector>
#include <optional>
#include <tuple>
#include <atomic>
#include <condition_variable>

#include "game_field.h"
#include "game_logic.h"
#include "game_output.h"
#include "bot.h"

#define BOT -1;

class GameRoom
{
private:
protected:
    TypeStatus  _status; 
public:
    const size_t      id;
    GameRoom(size_t x_id);
    virtual ~GameRoom() = default;
    virtual ReportAction DoAction(Player player, TypeAction type, DataAction data = {}) = 0;
    virtual GameResult GetResult() = 0;
    virtual std::vector<Player> GetPlayers() = 0;
    TypeStatus GetStatus();
    void Stop();

};

class T_Room : public GameRoom
{
private:
    inline static std::vector<size_t> _static_ids;
    std::optional<GameResult> result = std::nullopt;
    const TypeGame          _typeGame;
    const Player            _player_1;
    const Player            _player_2;
    T_GameField*            _field;
    const T_GameLogic*      _logic;
    const T_Bot*            _bot;
    T_Output*               _output;
    std::vector<StepInfo>  _steps;
    bool                    _logging;
    void fillReport(ReportAction &report, Player player, TypeAction type, DataAction data);
    void addStep(size_t player_id, size_t index, TypeCell cell);
    std::tuple<Player, Player, ReportAction> checkPlayer(Player player);
public:
    std::atomic<bool>       finish = false;
    std::atomic<bool>       busy = false;
    mutable std::mutex      mutex;
    T_Room(size_t room_id, Player player_1, Player player_2, T_GameField *field, T_GameLogic *logic, T_Output *output, T_Bot *bot = nullptr, TypeGame typeGame = TypeGame::OT, bool logging = false);
    virtual ReportAction DoAction(Player player, TypeAction type, DataAction data = {}) override;
    virtual std::vector<Player> GetPlayers() override;
    virtual GameResult GetResult() override;
};

template <typename T>
bool isHas(std::vector<T> const &items, T item)
{
    auto it = std::find(items.begin(), items.end(), item);
    return it == items.end() ? false : true;
}

