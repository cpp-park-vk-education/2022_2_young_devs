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
    std::lock_guard locker(_mutex);
    LogReport(report, "**[ REPORT ]**", _out);
    if (report.typeGame == TypeGame::OT)
    {
        OT_Output(report);
    }
    else if(report.typeGame == TypeGame::ST)
    {
        ST_Output(report);
    }
}


T_WtOutput::T_WtOutput(std::vector<Wt::WPushButton *> &cellButtons,
                       Wt::WPushButton *rollbackButton, Wt::WText *status):
        cellButtons_(cellButtons), rollbackButton_(rollbackButton), status_(status) {}
/*
std::vector<char> GetCurrentField(ReportAction report)
{
    std::vector<char> vecField(81);
    for (size_t i = 0; i < report.field->Size(); ++i)
    {
        switch(report.field->At(i))
        {
            case TypeCell::X:
            {
                vecField[convertToContinous] = 'X';
                break;
            }
            case TypeCell::O:
            {
                vecField[convertToContinous] = 'O';
                break;
            }
            case TypeCell::E:
            {
                vecField[convertToContinous] = '\0';
                break;
            }
        }
    }

    return vecField;
}
 */

size_t convertToContinous(size_t index)
{
     size_t block_no = index / 9;
     size_t inner_block_no = index % 9;
     size_t i = (block_no / 3 * 3) + inner_block_no / 3;
     size_t j = (block_no % 3 * 3) + inner_block_no % 3;
     return i * 9 + j;
}


std::vector<bool> GetDisabledButtons(ReportAction report) {
    size_t index = report.data.value;
    size_t block = index % 9;
    std::vector<bool> indices(81);
    for (size_t i = 0; i < 81; ++i) {
        if (i / 9 != block) {
            indices[convertToContinous(i)] = false;
        }
        else {
            indices[convertToContinous(i)] = true;
        }
    }
    return indices;
}

void T_WtOutput::Output(ReportAction report)
{
    //LogReport(report, "**[ REPORT ]**");
    // Анализ отчета
    // Обновление поля в браузере

    LogReport(report, "**[ REPORT ]**", std::cout);

    if (report.isValid)
    {
        if (report.result.isEnd) {
            if (report.result.draw) {
                status_->setText("Draw!");
            } else if (report.result.winner.isBot) {
                status_->setText("Bot won!");
            } else {
                status_->setText("You won!");
            }

            for (size_t i = 0; i < cellButtons_.size(); i++) {
                cellButtons_[i]->disable();
                std::cout << i << std::endl;
            }
        }

        if (report.player.cell == TypeCell::X) {
            cellButtons_[convertToContinous(report.data.value)]->setText("X");
        } else {
            cellButtons_[convertToContinous(report.data.value)]->setText("O");
        }

        size_t i = 0;
        std::vector<bool> indices = GetDisabledButtons(report);
        for (const auto &index: indices) {
            if (index) {
                cellButtons_[i]->enable();
            } else {
                cellButtons_[i]->disable();
            }
            i++;
        }
    }
}