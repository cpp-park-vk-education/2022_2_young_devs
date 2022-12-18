#pragma once

#include <iostream> 
#include <mutex> 

#include "game_logic.h"


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
    T_StreamOutput(std::string &file_name);
    virtual void Output(ReportAction report) override;
};

class T_WtOutput : public T_Output
{
public:
    virtual void Output(ReportAction report) override;
};