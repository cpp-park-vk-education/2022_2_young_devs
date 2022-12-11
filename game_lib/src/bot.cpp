#include "bot.h"
#include "game_field.h"

ReportAction OT_Bot::MakeStep(ReportAction &report)
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