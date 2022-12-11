#pragma once

#include "DataBase.h"

class Game
{
private:
    DataBase *database;

public:
    explicit Game(const std::string &ip = "localhost", const std::string &port = "3306",
                  const std::string &user = "cpp_dev", const std::string &password = "123",
                  const std::string &db_name = "cpp_project_db");

    void addGame(size_t user1_id, size_t user2_id);
    void updateGameStatus(size_t game_id, const std::string &new_status);
    void updateGameWinner(size_t game_id, size_t winner_id);
    void deleteGame(size_t game_id);
    size_t userGames(size_t user_id);
    size_t userWins(size_t user_id);
    std::pair<size_t, size_t> getPlayers(size_t game_id);
};
