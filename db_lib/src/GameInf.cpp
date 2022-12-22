#include "GameInf.h"


GameInf::GameInf(const std::string &ip, const std::string &port, const std::string &user, const std::string &password,
           const std::string &db_name)
{
    database = new DataBase(ip, port, user, password, db_name);
}

void GameInf::addGame(size_t user1_id, size_t user2_id, const std::string& status, const std::string& type, int game_id)
{
    if (user1_id == user2_id)
    {
        return;
    }
    if (game_id == -1)
    {
        database->Insert("INSERT INTO Game(user1_id, user2_id, status, type) VALUES (?, ?, ?, ?)",
                         {{"I:" + std::to_string(user1_id), "I:" + std::to_string(user2_id),
                           "S:" + status, "S:" + type}});
    }
    else
    {
        database->Insert("INSERT INTO Game(id, user1_id, user2_id, status, type) VALUES (?, ?, ?, ?, ?)",
                         {{"I:" + std::to_string(game_id), "I:" + std::to_string(user1_id),
                           "I:" + std::to_string(user2_id), "S:" + status, "S:" + type}});
    }
}

void GameInf::updateGameStatus(size_t game_id, const std::string &new_status)
{
    database->Update("UPDATE Game SET status=? WHERE id=?",
                     {"S:" + new_status, "I:" + std::to_string(game_id)});
}

void GameInf::updateGameWinner(size_t game_id, size_t winner_id)
{
    updateGameStatus(game_id, "finished");
    database->Update("UPDATE Game SET winner_id=? WHERE id=?",
               {"I:" + std::to_string(winner_id), "I:" + std::to_string(game_id)});
}

void GameInf::deleteGame(size_t game_id)
{
    database->Delete("DELETE FROM Game WHERE id=?", {"I:" + std::to_string(game_id)});
}

GameTable GameInf::getGameInfo(size_t game_id)
{
    auto res = database->Select("SELECT * FROM Game WHERE id=?",
                                {"I:" + std::to_string(game_id)});
    return GameTable(res[0]);
}

std::vector<GameTable> GameInf::getUserGames(size_t user_id)
{
    std::vector<GameTable> games;
    auto res = database->Select("SELECT * FROM Game WHERE user1_id=? OR user2_id=?",
                                {"I:" + std::to_string(user_id), "I:" + std::to_string(user_id)});
    for (auto & re : res)
    {
        games.emplace_back(re);
    }
    return games;
}

size_t GameInf::userGames(size_t user_id)
{
    auto num_Games =  database->Select("SELECT count(*) FROM Game WHERE user1_id=? OR user2_id=?",
                            {"I:" + std::to_string(user_id), "I:" + std::to_string(user_id)});
    return std::stoi(num_Games[0][0]);
}

size_t GameInf::userWins(size_t user_id)
{
    auto num_wins = database->Select("SELECT count(*) FROM Game WHERE winner_id=?",
                                     {"I:" + std::to_string(user_id)});
    return std::stoi(num_wins[0][0]);
}

std::pair<size_t, size_t> GameInf::getPlayers(size_t game_id)
{
    auto players = database->Select("SELECT user1_id, user2_id FROM Game WHERE id=?",
                                    {"I:" + std::to_string(game_id)});
    return {std::stoi(players[0][0]), std::stoi(players[0][1])};
}

std::string GameInf::getGameType(size_t game_id)
{
    auto res = database->Select("SELECT type FROM Game WHERE id=?",
                                {"I:" + std::to_string(game_id)});
    return res[0][0];
}

bool GameInf::stoppedGameExist(size_t user_id)
{
    auto res = database->Select("SELECT id FROM Game WHERE status='stopped' AND (user1_id=? OR user2_id=?)",
                                {"I:" + std::to_string(user_id), "I:" + std::to_string(user_id)});
    if (res.empty())
    {
        return false;
    }
    return true;
}
