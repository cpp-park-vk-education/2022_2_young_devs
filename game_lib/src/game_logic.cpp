#include "game_logic.h"
#include "game_field.h"
#include "reports_bug.h"

using namespace std::string_literals;


ReportAction OT_Logic::MakeStep(ReportAction const &report)
{
    auto field = report.field;
    size_t index = report.data.value;
    TypeCell cell = report.player.cell;

    size_t dim = field->Dimension();
<<<<<<< HEAD
    if (index >= (dim * dim))
    {
        // индекс за пределами игрового поля или индекс занятой ячейки
        ReportAction report = reportCode1;
        report.result = field->IsEnd();
        return report;
    }
    if (field->At(index) != TypeCell::E)
    {
        ReportAction report = reportCode2;
        report.result = field->IsEnd();
        return report;
    }
    field->Set(index, cell);
||||||| 3cfaff3
	if (index >= (dim * dim))
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		return { 
			.isValid = false, 
			.error = { 
				.codeError = 1, 
				.messageError = "index out of range",
			}, 
			.result = field->IsEnd() 
		};
	}
	if (field->At(index) != TypeCell::E)
	{
		return { 
			.isValid = false, 
			.error = { 
				.codeError = 2, 
				.messageError = "cell is not empty",
			}, 
			.result = field->IsEnd() 
		};
	}
	field->Set(index, cell);
=======
	if (index >= (dim * dim))
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		ReportAction report = reportCode1;
		report.result = field->IsEnd();
		return report;
	}
	if (field->At(index) != TypeCell::E)
	{
		ReportAction report = reportCode2;
		report.result = field->IsEnd();
		return report;
	}
	field->Set(index, cell);
>>>>>>> 02cb01ac6d1f1c2a279a7b463ad66af66cff0bc8
    return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
}


ReportAction ST_Logic::MakeStep(ReportAction const &report)
{
    auto field = report.field;
    size_t index = report.data.value;
    TypeCell cell = report.player.cell;
    size_t dim = field->Dimension();

<<<<<<< HEAD
    if ((index / 9) >= (dim))
    {
        // индекс за пределами игрового поля или индекс занятой ячейки
        ReportAction report = reportCode1;
        report.result = field->IsEnd();
        return report;
    }
    if (field->At(index) != TypeCell::E)
    {
        ReportAction report = reportCode2;
        report.result = field->IsEnd();
        return report;
    }

    if (report.steps.empty())
    {
        field->Set(index, cell);
    }
    else
    {
        size_t index_before = report.steps.back().index;
        if (index / 9 == index_before % 9)
        {
            field->Set(index, cell);
        }
        else
        {
            ReportAction report = reportCode9;
            report.result = field->IsEnd();
            return report;
        }
    }
    return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
||||||| 3cfaff3
	if ((index / 9) >= (dim))
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		return { 
			.isValid = false, 
			.error = { 
				.codeError = 1, 
				.messageError = "index out of range",
			}, 
			.result = field->IsEnd() 
		};
	}
	if (field->At(index) != TypeCell::E)
	{
		return { 
			.isValid = false, 
			.error = { 
				.codeError = 2, 
				.messageError = "cell is not empty",
			}, 
			.result = field->IsEnd() 
		};
	}
	
	if (report.steps.empty())
	{
		field->Set(index, cell);
	}
	else
	{
		size_t index_before = report.steps.back().index;
		if (index / 9 == index_before % 9)
		{
			field->Set(index, cell);
		}
		else
		{
			return { 
				.isValid = false, 
				.error = { 
					.codeError = 10, 
					.messageError = "Your move should have been in the "s + std::to_string(index_before / 9) + "th field 3x3",
				}, 
				.result = field->IsEnd() 
			};
		}
	}
	return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
=======
	if ((index / 9) >= (dim))
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		ReportAction report = reportCode1;
		report.result = field->IsEnd();
		return report;
	}
	if (field->At(index) != TypeCell::E)
	{
		ReportAction report = reportCode2;
		report.result = field->IsEnd();
		return report;
	}
	
	if (report.steps.empty())
	{
		field->Set(index, cell);
	}
	else
	{
		size_t index_before = report.steps.back().index;
		if (index / 9 == index_before % 9)
		{
			field->Set(index, cell);
		}
		else
		{
			ReportAction report = reportCode9;
			report.result = field->IsEnd();
			return report;
		}
	}
	return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
>>>>>>> 02cb01ac6d1f1c2a279a7b463ad66af66cff0bc8
}