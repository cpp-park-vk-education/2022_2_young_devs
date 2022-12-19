#include "game_logic.h"
#include "game_field.h"
#include "reports_bug.h"

using namespace std::string_literals;


ReportAction OT_Logic::MakeStep(ReportAction const &report) const
{
	auto field = report.field;
	size_t index = report.data.value;
	TypeCell cell = report.player.cell;

    size_t dim = field->Dimension();
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
    return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
}


ReportAction ST_Logic::MakeStep(ReportAction const &report) const
{
	auto field = report.field;
	size_t index = report.data.value;
	TypeCell cell = report.player.cell;
	size_t dim = field->Dimension();

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
	
	size_t i_tmp = 0;
	if (!report.steps.empty())
	{
		size_t needed_block = report.steps.back().index % 9;
		for (; i_tmp < 9; ++i_tmp)
		{
			if (field->At(needed_block * 9 + i_tmp) == TypeCell::E)
			{
				break;
			}
		}
	}

	if (report.steps.empty() || i_tmp == 9)
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
}