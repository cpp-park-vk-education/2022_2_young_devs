#pragma once

#include "DataBase.h"
#include "GameInf.h"

enum TypeCell { X = 1, E = 0, O = -1};

struct StepInfo
{
    size_t id_player;
    size_t index;
    TypeCell cell;
};

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
                 const std::string &user = "cpp_dev", const std::string &password = "123",
                 const std::string &db_name = "cpp_project_db");

    void addMoves(size_t game_id, const std::vector<StepInfo>& steps);
    getSteps getMoves(size_t game_id);
    void deleteMove(size_t id);
    void deleteLastMoves(size_t game_id, size_t user_id = 0, size_t num = 0);
};
