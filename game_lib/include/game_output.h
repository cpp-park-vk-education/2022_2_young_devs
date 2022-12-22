#pragma once

#include <iostream> 
#include <mutex>
#include <vector>

#include "game_logic.h"

#include <Wt/WPushButton.h>
#include <Wt/WText.h>


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
    T_WtOutput(std::vector<Wt::WPushButton *> &cellButtons,
               Wt::WPushButton *rollbackButton, Wt::WText *status);
    virtual void Output(ReportAction report) override;

private:
    std::vector<Wt::WPushButton *> &cellButtons_;

    Wt::WPushButton *rollbackButton_;

    Wt::WText *status_;

    void isEnd(const ReportAction &report);

    void disableButtons(const ReportAction &report);
};