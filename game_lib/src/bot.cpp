#include <cstdlib>

#include "bot.h"
#include "game_field.h"
#include "helpers_func.h"


// ходит в первую пустую ячейку
ReportAction OT_Bot::MakeStep(ReportAction &report) const
{
    auto field = report.field;
    TypeCell cell = report.player.cell;
    size_t i = 0;
    for (; i < field->Size(); ++i)
    {
        if (field->At(i) == E)
        {
            field->Set(i, cell);
            break;
        }
    }
    return { .data = {.value = i }, .isValid = true, .result = field->IsEnd() };
}

// ходит в случайную пустую ячейку в нужном поле 3x3
ReportAction ST_Bot::MakeStep(ReportAction &report) const
{
	auto field = report.field;
	TypeCell cell = report.player.cell;
	if (report.steps.empty())
	{
		size_t index = get_rand(0, 80);
		field->Set(index, cell);
		return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
	}
	else
	{
		size_t last_index = report.steps.back().index;
		size_t start_index_fiend = (last_index % 9) * 9; 
		std::vector<size_t> empty_cells;
		for (size_t i = start_index_fiend; i < start_index_fiend + 9; ++i)
		{
			if (field->At(i) == TypeCell::E)
			{
				empty_cells.push_back(i);
			}
		}
		size_t new_index = empty_cells[get_rand(0, empty_cells.size() - 1)];
		// size_t new_index = empty_cells[empty_cells.size() / 2];
		field->Set(new_index, cell);
		return { .data = {.value = new_index }, .isValid = true, .result = field->IsEnd() };
	}
}
