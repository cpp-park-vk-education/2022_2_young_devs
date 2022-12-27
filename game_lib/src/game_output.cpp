#include <iomanip>
#include <nlohmann/json.hpp>

#include "game_output.h"
#include "game_field.h"
#include "helpers_func.h"


T_StreamOutput::T_StreamOutput(std::ostream &out) : _out(out)
{
}

T_StreamOutput::T_StreamOutput(std::ostream &&out) : _out(out)
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
    std::string strJs = strJson(report);
    _out << strJs << std::endl;
    nlohmann::json j = nlohmann::json::parse(strJs);
    _out << j["room_id"] << std::endl;
    _out << j["message_error"] << std::endl;
    _out << j["field"][0] << std::endl;
    // _out << j["steps"][0]["index"] << std::endl;
    // LogReport(report, "**[ REPORT ]**", _out);
    // _out << "==========[ " <<  report.room_id << " ]==========\n";
    if (report.typeGame == TypeGame::OT)
    {
        OT_Output(report);
    }
    else if(report.typeGame == TypeGame::ST)
    {
        ST_Output(report);
    }
    _out << "==========================\n\n\n";
}

/*
    convertToBlocks и convertToContinous - конвертация индексов

    1 2 3 | 10 11 12 | 19 20 21        1  2  3  4  5  6  7  8  9
    4 5 6 | 13 14 15 | 22 23 24   ->   10 11 12 13 14 15 16 17 18
    7 8 9 | 16 17 18 | 25 26 27        19 20 21 22 23 24 25 26 27
    
    Только начинаются с нуля
*/

/*
    size_t convertToContinous(size_t index)
    {
        size_t block_no = index / 9;
        size_t inner_block_no = index % 9;
        size_t i = (block_no / 3 * 3) + inner_block_no / 3; 
        size_t j = (block_no % 3 * 3) + inner_block_no % 3;
        return i * 9 + j;
    }

    size_t convertToBlocks (size_t index)
    {
        size_t i = index / 9;
        size_t j = index % 9;
        size_t start_index_in_block = ((i / 3) * 3 + (j / 3)) * 9;
        return start_index_in_block + (i % 3) * 3 + (j % 3);
    }
*/


void T_WtOutput::Output(ReportAction report)
{
    // Анализ отчета
    // Вывод поля в браузер
}

std::mutex test_output_mutex;

void T_TestOutput::Output(ReportAction report)
{
    std::lock_guard locker(test_output_mutex);
    if (report.result.isEnd)
    {
        _streamOutput.Output(report);
    }
}