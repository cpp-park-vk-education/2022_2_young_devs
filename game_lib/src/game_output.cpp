#include <iomanip>
#include <iostream>

#include "game_output.h"
#include "game_field.h"
#include "helpers_func.h"


T_StreamOutput::T_StreamOutput(std::ostream &out) : _out(out)
{
}

void T_StreamOutput::OT_Output(ReportAction report)
{
    for (size_t i = 0; i < report.field->Size(); ++i)
    {
        std::string strCell = TypeCellStr(report.field->At(i));
        _out << std::setw(2) << strCell << ' ';
        if (i % report.field->Dimension() == report.field->Dimension() - 1)
        {
            _out << '\n';
        }
    }
}

// ПРИГОДИТСЯ

// size_t convertToContinous(size_t index)
// {
//     size_t block_no = index / 9;
//     size_t inner_block_no = index % 9;
//     size_t i = (block_no / 3 * 3) + inner_block_no / 3; 
//     size_t j = (block_no % 3 * 3) + inner_block_no % 3;
//     return i * 9 + j;
// }

void T_StreamOutput::ST_Output(ReportAction report)
{
    auto convertToBlocks = [](size_t index){
        size_t i = index / 9;
        size_t j = index % 9;
        size_t start_index_in_block = ((i / 3) * 3 + (j / 3)) * 9;
        return start_index_in_block + (i % 3) * 3 + (j % 3);
    };

    for (size_t i = 0; i < report.field->Size(); ++i)
    {
        size_t index = convertToBlocks(i);
        std::string strCell = TypeCellStr(report.field->At(index));
        _out << std::setw(2) << strCell << ' ';
        if (i % report.field->Dimension() == report.field->Dimension() - 1)
        {
            _out << '\n';
        }
    }
}

void T_StreamOutput::Output(ReportAction report)
{
    LogReport(report, "**[ REPORT ]**");
    if (report.typeGame == TypeGame::OT)
    {
        OT_Output(report);
    }
    else if(report.typeGame == TypeGame::ST)
    {
        ST_Output(report);
    }
}

T_WtOutput::T_WtOutput(std::vector<Wt::WPushButton *> &cellButtons_):
        cellButtons_(cellButtons_){}

size_t convertToContinous(size_t index)
{
     size_t block_no = index / 9;
     size_t inner_block_no = index % 9;
     size_t i = (block_no / 3 * 3) + inner_block_no / 3;
     size_t j = (block_no % 3 * 3) + inner_block_no % 3;
     return i * 9 + j;
}


void T_WtOutput::Output(ReportAction report)
{
    // Анализ отчета
    // Обновление поля в браузере

    if (report.isValid) {
        std::cout << "DATA VALUE " << report.data.value << std::endl;
        std::cout << "ХОД БОТА " << convertToContinous(report.data.value) << std::endl;

        cellButtons_[convertToContinous(report.data.value)]->setText("1");
    }
}