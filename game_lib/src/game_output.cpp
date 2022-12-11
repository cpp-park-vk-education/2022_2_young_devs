#include <iomanip>

#include "game_output.h"
#include "game_field.h"
#include "helpers_func.h"


T_StreamOutput::T_StreamOutput(std::ostream &out) : _out(out)
{
}

void T_StreamOutput::Output(ReportAction report)
{
    LogReport(report);
    for (size_t i = 0; i < report.field->Size(); ++i)
    {
        std::string strCell;
#define PRINT(a) strCell = #a;
		switch (report.field->At(i))
		{
            case X: {
                PRINT(X);
                break;
            }
            case O: {
                PRINT(O);
                break;
            }
            case E: {
                strCell = "-";
                break;
            }
            default: {
                strCell = "~";
            }
		}
		_out << std::setw(2) << strCell << ' ';
		if (i % report.field->Dimension() == report.field->Dimension() - 1)
		{
			_out << '\n';
		}
    }
    // Анализ отчета
    // Вывод поля в консоль
}

void T_WebToolKitOutput::Output(ReportAction report)
{
    // Анализ отчета
    // Обновление поля в браузере
}