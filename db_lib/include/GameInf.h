#pragma once

#include "DataBase.h"

struct GameTable
{
    explicit GameTable(std::vector<std::string> data)
    {
        id = std::stoi(data[0]);
        user1_id = std::stoi(data[1]);
        user2_id = std::stoi(data[2]);
        status = data[3];
        winner_id = std::stoi(data[4]);
        type = data[5];
        start = data[6];
    }

    size_t id;
    size_t user1_id;
    size_t user2_id;
    std::string status;
    size_t winner_id;
    std::string type;
    std::string start;
};

class GameInf
{
private:
    DataBase *database;

public:
    explicit GameInf(const std::string &ip = "localhost", const std::string &port = "3306",
                  const std::string &user = "root", const std::string &password = "dimasdr2002",
                  const std::string &db_name = "cpp_project_db");

    void addGame(size_t user1_id, size_t user2_id, const std::string& status, const std::string& type, int game_id = -1);
    void updateGameStatus(size_t game_id, const std::string &new_status);
    void updateGameWinner(size_t game_id, size_t winner_id);
    void deleteGame(size_t game_id);
    GameTable getGameInfo(size_t game_id);
    std::vector<GameTable> getUserGames(size_t user_id);
    bool stoppedGameExist(size_t user_id);
    size_t userGames(size_t user_id);
    size_t userWins(size_t user_id);
    std::pair<size_t, size_t> getPlayers(size_t game_id);
    std::string getGameType(size_t game_id);
};
