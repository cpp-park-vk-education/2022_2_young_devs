#include "game_logic.h"
#include "game_field.h"

ReportAction OT_Logic::MakeStep(TypeCell cell, size_t index, T_GameField *field)
{
    size_t dim = field->Dimension();
	if (index >= (dim * dim) || field->At(index) != TypeCell::E)
	{
		// индекс за пределами игрового поля или индекс занятой ячейки
		return { .isValid = false };
	}
	field->Set(index, cell);
    return { .isValid = true };
}

ReportAction ST_Logic::MakeStep(TypeCell cell, size_t index, T_GameField *field)
{
    // TODO
}