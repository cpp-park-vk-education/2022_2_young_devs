#pragma once

#include <cstddef>
#include <string>

class T_GameField;

enum class TypeStatus   { Active = 0, Stopped, Finished };
enum class TypeAction   { Step = 0, Rollback, Stop };
enum class TypeGame     { OT = 0, ST };
enum       TypeCell     { X = 1, E = 0, O = -1 };

struct GameResult
{
    bool isEnd;
    TypeCell winner;
    size_t winner_id;
    bool draw;
};

struct ReportError
{
    size_t          _codeError;
    std::string     _messageError;
};

struct DataAction
{
    int value;
    std::string data;
};

struct ReportAction
{
    // чей action
    size_t          player_id;
    // что было на входе action
    DataAction      data;
    TypeGame        typeGame;
    TypeAction      typeAction;
    // статус игры после action
    TypeStatus      status;
    bool            isValid;
    ReportError     error;
    T_GameField*    field;
    GameResult      result;
};

class T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(TypeCell cell, size_t index, T_GameField *field) = 0;
};

class OT_Logic : public T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(TypeCell cell, size_t index, T_GameField *field) override;
};

class ST_Logic : public T_GameLogic
{
private:
public:
    virtual ReportAction MakeStep(TypeCell cell, size_t index, T_GameField *field) override;
};