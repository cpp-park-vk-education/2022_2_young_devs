#include "GameProgress.h"


GameProgress::GameProgress(const std::string &ip, const std::string &port, const std::string &user,
                           const std::string &password, const std::string &db_name)
{
    database = new DataBase(ip, port, user, password, db_name);
}

void GameProgress::addMoves(size_t game_id, const std::vector<StepInfo>& steps)
{
    std::vector<std::vector<std::string>> moves;
    for (const auto &step: steps)
    {
        if (step.cell == 1)
        {
            moves.push_back({"I:" + std::to_string(game_id), "S:X", "I:" + std::to_string(step.index)});
        }
        else if (step.cell == -1)
        {
            moves.push_back({"I:" + std::to_string(game_id), "S:0", "I:" + std::to_string(step.index)});
        }
    }
    database->Insert("INSERT INTO GameProgress(game_id, sign, cell) VALUES (?, ?, ?)", moves);
}

getSteps GameProgress::getMoves(size_t game_id)
{
    auto res = database->Select("SELECT sign, cell FROM GameProgress WHERE game_id=?",
                                {"I:" + std::to_string(game_id)});
    getSteps moves;
    std::vector<StepInfo> steps;
    for (const auto &step: res)
    {
        if (step[0] == "X")
        {
            steps.push_back(StepInfo{0, std::stoul(step[1]), X});
        }
        else
        {
            steps.push_back(StepInfo{1, std::stoul(step[1]), O});
        }
    }
    moves.steps = steps;
    GameInf game;
    auto players = game.getPlayers(game_id);
    if (players.first == 1 || players.second == 1)
    {
        moves.game_with_bot = true;
    }
    else
    {
        moves.game_with_bot = false;
    }
    moves.type = game.getGameType(game_id);
    return moves;
}

void GameProgress::deleteMove(size_t id)
{
    database->Delete("DELETE FROM GameProgress WHERE id=?", {"I:" + std::to_string(id)});
}

void GameProgress::deleteLastMoves(size_t game_id, size_t user_id, size_t num)
{
    std::vector<std::vector<std::string>> res;
    if (user_id == 0)
    {
        if (num == 0)
        {
            res = database->Select("SELECT id FROM GameProgress WHERE game_id=? ORDER BY id DESK LIMIT 1",
                                   {"I:" + std::to_string(game_id)});
        }
        else
        {
            res = database->Select("SELECT id FROM GameProgress WHERE game_id=? ORDER BY id DESK LIMIT ?",
                                   {"I:" + std::to_string(game_id), "I:" + std::to_string(num)});
        }
    }
    else
    {
        if (num == 0)
        {
            res = database->Select("SELECT id FROM GameProgress WHERE game_id=? AND user_id=?"
                                   "ORDER BY id DESK LIMIT 1",
                                   {"I:" + std::to_string(game_id), "I:" + std::to_string(user_id)});
        }
        else
        {
            res = database->Select("SELECT id FROM GameProgress WHERE game_id=? AND user_id=?"
                                   "ORDER BY id DESC LIMIT ?",
                                   {"I:" + std::to_string(game_id), "I:" + std::to_string(user_id),
                                    "I:" + std::to_string(num)});
        }
    }
    for (size_t i = 0; i < res.size(); i++)
    {
        deleteMove(std::stoi(res[0][0]));
    }
}
