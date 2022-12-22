#pragma once

#include "DataBase.h"
#include "GameInf.h"

#include "../../game_lib/include/game_logic.h"

struct getSteps
{
    bool game_with_bot;
    std::string type;
    std::vector<StepInfo> steps;
};

class GameProgress
{
private:
    DataBase *database;

public:
    explicit GameProgress(const std::string &ip = "localhost", const std::string &port = "3306",
                 const std::string &user = "root", const std::string &password = "dimasdr2002",
                 const std::string &db_name = "cpp_project_db");

    void addMoves(size_t game_id, const std::vector<StepInfo>& steps);
    getSteps getMoves(size_t game_id);
    std::vector<size_t> getPlayerMoves(size_t game_id, size_t user_id, size_t num = 0);
    void deleteMove(size_t id);
    void deleteLastMoves(size_t game_id, size_t user_id = 0, size_t num = 0);
};
