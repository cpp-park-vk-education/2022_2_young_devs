#pragma once

#include <iostream> 
#include <mutex>
#include <vector>

#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include "game_logic.h"
#include "../../db_lib/include/GameInf.h"

class T_Output
{
public:
    virtual void Output(ReportAction report) = 0;
};

class T_StreamOutput : public T_Output
{
private:
    mutable std::mutex _mutex;
    std::ostream &_out;
    void OT_Output(ReportAction report);
    void ST_Output(ReportAction report);
public:
    T_StreamOutput(std::ostream &out = std::cout);
    virtual void Output(ReportAction report) override;
};

class T_WtOutput : public T_Output
{
public:
    T_WtOutput(std::vector<Wt::WPushButton *> &cellButtons, Wt::WPushButton *rollbackButton,
               Wt::WText *status, Wt::WPushButton *newGameButton, GameInf &gameInf);
    virtual void Output(ReportAction report) override;

private:
    std::vector<Wt::WPushButton *> &cellButtons_;

    GameInf gameInf_;

    Wt::WPushButton *rollbackButton_;

    Wt::WText *status_;

    Wt::WPushButton *newGameButton_;

    void isEnd(const ReportAction &report);

    void disableButtons(const ReportAction &report);
};