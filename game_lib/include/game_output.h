#pragma once

#include <iostream> 
#include <mutex> 
#include <fstream> 

#include "game_logic.h"
#include "config.h"


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
    T_StreamOutput(std::ostream &&out);
    virtual void Output(ReportAction report) override;
};

class T_WtOutput : public T_Output
{
public:
    virtual void Output(ReportAction report) override;
};

class T_NoOutput : public T_Output
{
private:
    T_StreamOutput _streamOutput;
    inline static std::ofstream _file{BASE_DIR + std::string("/tests/test_files/results.out")};
public:
    T_NoOutput() : _streamOutput(_file) {}
    virtual void Output(ReportAction report) override;
};