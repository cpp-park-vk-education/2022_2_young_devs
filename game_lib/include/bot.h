#pragma once

#include "game_logic.h"

class T_Bot
{
public:
    virtual ReportAction MakeStep(ReportAction &report) = 0;
};

class OT_Bot : public T_Bot
{
public:
    virtual ReportAction MakeStep(ReportAction &report) override;
};

class ST_Bot : public T_Bot
{
public:
    virtual ReportAction MakeStep(ReportAction &report) override;
};

