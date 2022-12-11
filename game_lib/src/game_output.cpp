#include <iomanip>

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

void T_StreamOutput::ST_Output(ReportAction report)
{
    std::vector<char> indices =
    {
        0,  1,   2,  9,  10, 11, 18, 19, 20,
        3,  4,   5,  12, 13, 14, 21, 22, 23, 
        6,  7,   8,  15, 16, 17, 24, 25, 26,
        27, 28,  29, 36, 37, 38, 45, 46, 47,
        30, 31,  32, 39, 40, 41, 48, 49, 50,
        33, 34,  35, 42, 43, 44, 51, 52, 53,
        54, 55,  56, 63, 64, 65, 72, 73, 74,
        57, 58,  59, 66, 67, 68, 75, 76, 77,
        60, 61,  62, 69, 70, 71, 78, 79, 80
    };
    for (size_t i = 0; i < indices.size(); ++i)
    {
        size_t index = indices[i];
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
    
    // Анализ отчета
    // Вывод поля в консоль
}

void T_WebToolKitOutput::Output(ReportAction report)
{
    // Анализ отчета
    // Обновление поля в браузере
}