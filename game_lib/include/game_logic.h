#pragma once


#include <cstddef>
#include <string>
#include <vector>

class T_GameField;

enum class TypeStatus   { Active = 0, Stopped, Finished };
enum class TypeAction   { Step = 0, Rollback };
enum class TypeGame     { OT = 0, ST };
enum       TypeCell     { X = 1, E = 0, O = -1 };

struct StepInfo
{
    ssize_t     player_id;
    size_t      index;
    TypeCell    cell;
};

struct Player
{
    // если бот, так и остается
    ssize_t     id = -1;
    bool        isBot = false;
    TypeCell    cell;
};

struct GameResult
{
    bool        isEnd = false;
    TypeCell    winnerCell;
    Player      winner;
    bool        draw = false;
};

struct ReportError
{
    size_t          codeError;
    std::string     messageError;
};

struct DataAction
{
    int         value;
    std::string data;
};

struct ReportAction
{
    size_t          room_id;
    // чей action
    Player          player;
    // что было на входе action
    DataAction      data;
    TypeGame        typeGame;
    TypeAction      typeAction;
    // статус игры после action
    TypeStatus      status;
    bool            isValid = false;
    ReportError     error;
    T_GameField*    field;
    GameResult      result;
    std::vector<StepInfo> steps;
};

class T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(ReportAction const &report) const = 0;
};

class OT_Logic : public T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(ReportAction const &report) const override;
};

class ST_Logic : public T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(ReportAction const &report) const override;
};