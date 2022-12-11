#include "game_logic.h"

class T_Bot
{
public:
    virtual ReportAction MakeStep(T_GameField *field, TypeCell cell) = 0;
};

class OT_Bot : public T_Bot
{
public:
    virtual ReportAction MakeStep(T_GameField *field, TypeCell cell) override;
};


