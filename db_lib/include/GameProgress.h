#pragma once

#include "DataBase.h"

class GameProgress
{
private:
    DataBase *database;

public:
    explicit GameProgress(const std::string &ip = "localhost", const std::string &port = "3306",
                 const std::string &user = "cpp_dev", const std::string &password = "123",
                 const std::string &db_name = "cpp_project_db");
    ~GameProgress();

    void addMove(size_t game_id, size_t user, size_t cell);
    std::vector<std::pair<size_t, size_t>> getMoves(size_t game_id, size_t num = 0);
    std::vector<size_t> getPlayerMoves(size_t game_id, size_t user_id, size_t num = 0);
    void deleteMove(size_t id);
    void deleteLastMoves(size_t game_id, size_t user_id = 0, size_t num = 0);
};
