#include "GameProgress.h"


GameProgress::GameProgress(const std::string &ip, const std::string &port, const std::string &user,
                           const std::string &password, const std::string &db_name)
{
    database = new DataBase(ip, port, user, password, db_name);
}

void GameProgress::addMove(size_t game_id, size_t user_id, size_t cell)
{
    database->Insert("INSERT INTO GameProgress(game_id, user_id, cell) VALUES (?, ?, ?)",
                     {{"I:" + std::to_string(game_id), "I:" + std::to_string(user_id),
                      "I:" + std::to_string(cell)}});
}

std::vector<std::pair<size_t, size_t>> GameProgress::getMoves(size_t game_id, size_t num)
{
    std::vector<std::pair<size_t, size_t>> moves;
    std::vector<std::vector<std::string>> res;
    if (num == 0)
    {
        res = database->Select("SELECT user_id, cell FROM GameProgress WHERE game_id=?",
                                    {"I:" + std::to_string(game_id)});
    }
    else
    {
        res = database->Select("SELECT user_id, cell FROM "
                                    "(SELECT user_id, cell FROM GameProgress WHERE game_id=? ORDER BY id DESC LIMIT ?)"
                                    "ORDER BY id",
                                    {"I:" + std::to_string(game_id), "I:" + std::to_string(num)});
    }
    for (auto & re : res)
    {
        moves.emplace_back(std::stoi(re[0]), std::stoi(re[1]));
    }
    return moves;
}

std::vector<size_t> GameProgress::getPlayerMoves(size_t game_id, size_t user_id, size_t num)
{
    std::vector<size_t> moves;
    std::vector<std::vector<std::string>> res;
    if (num == 0)
    {
        res = database->Select("SELECT user_id, cell FROM GameProgress "
                                    "WHERE game_id=? AND user_id=?",
                                    {"I:" + std::to_string(game_id), "I:" + std::to_string(user_id)});
    }
    else
    {
        res = database->Select("SELECT user_id, cell FROM "
                                    "(SELECT user_id, cell FROM GameProgress WHERE game_id=? AND user_id=?"
                                    "ORDER BY id DESC LIMIT ?) ORDER BY id",
                                    {"I:" + std::to_string(game_id), "I:" + std::to_string(user_id),
                                     "I:" + std::to_string(num)});
    }
    for (auto & re : res)
    {
        moves.emplace_back(std::stoi(re[0]));
    }
    return moves;
}

void GameProgress::deleteMove(size_t id)
{
    database->Delete("DELETE FROM GameProgress WHERE id=?", {"I:" + std::to_string(id)});
}

void GameProgress::deleteLastMove(size_t game_id, size_t user_id)
{
    auto res = getPlayerMoves(game_id, user_id, 1);
    deleteMove(res[0]);
}
