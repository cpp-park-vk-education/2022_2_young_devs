#include "GameInf.h"


GameInf::GameInf(const std::string &ip, const std::string &port, const std::string &user, const std::string &password,
           const std::string &db_name)
{
    database = new DataBase(ip, port, user, password, db_name);
}

void GameInf::addGame(size_t user1_id, size_t user2_id)
{
    if (user1_id == user2_id)
    {
        return;
    }
    database->Insert("INSERT INTO Game(user1_id, user2_id) VALUES (?, ?)",
                     {{"I:" + std::to_string(user1_id), "I:" + std::to_string(user2_id)}});
}

void GameInf::updateGameStatus(size_t GameInf_id, const std::string &new_status)
{
    database->Update("UPDATE Game SET status=? WHERE id=?",
                     {"S:" + new_status, "I:" + std::to_string(GameInf_id)});
}

void GameInf::updateGameWinner(size_t GameInf_id, size_t winner_id)
{
    database->Update("UPDATE Game SET winner_id=? WHERE id=?",
               {"I:" + std::to_string(winner_id), "I:" + std::to_string(GameInf_id)});
}

void GameInf::deleteGame(size_t GameInf_id)
{
    database->Delete("DELETE FROM Game WHERE id=?", {"I:" + std::to_string(GameInf_id)});
}

GameTable GameInf::getGameInfo(size_t GameInf_id)
{
    auto res = database->Select("SELECT * FROM Game WHERE id=?",
                                {"I:" + std::to_string(GameInf_id)});
    return GameTable(res[0]);
}

std::vector<GameTable> GameInf::getUserGames(size_t user_id)
{
    std::vector<GameTable> GameInfs;
    auto res = database->Select("SELECT * FROM Game WHERE user1_id=? OR user2_id=?",
                                {"I:" + std::to_string(user_id), "I:" + std::to_string(user_id)});
    for (auto & re : res)
    {
        GameInfs.emplace_back(re);
    }
    return GameInfs;
}

size_t GameInf::userGames(size_t user_id)
{
    auto num_GameInfs =  database->Select("SELECT count(*) FROM Game WHERE user1_id=? OR user2_id=?",
                            {"I:" + std::to_string(user_id), "I:" + std::to_string(user_id)});
    return std::stoi(num_GameInfs[0][0]);
}

size_t GameInf::userWins(size_t user_id)
{
    auto num_wins = database->Select("SELECT count(*) FROM Game WHERE winner_id=?",
                                     {"I:" + std::to_string(user_id)});
    return std::stoi(num_wins[0][0]);
}

std::pair<size_t, size_t> GameInf::getPlayers(size_t GameInf_id)
{
    auto players = database->Select("SELECT user1_id, user2_id FROM Game WHERE id=?",
                                    {"I:" + std::to_string(GameInf_id)});
    return {std::stoi(players[0][0]), std::stoi(players[0][1])};
}
