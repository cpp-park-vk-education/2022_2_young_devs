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

<<<<<<< HEAD
class ST_Bot : public T_Bot
{
public:
    virtual ReportAction MakeStep(ReportAction &report) override;
};
||||||| 3cfaff3
=======
class ST_Bot : public T_Bot
{
public:
    virtual ReportAction MakeStep(ReportAction &report) override;
};

>>>>>>> 02cb01ac6d1f1c2a279a7b463ad66af66cff0bc8

