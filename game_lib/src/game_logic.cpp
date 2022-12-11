#include "game_logic.h"
#include "game_field.h"

ReportAction OT_Logic::MakeStep(TypeCell cell, size_t index, T_GameField *field)
{
    size_t dim = field->Dimension();
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
    return { .data = {.value = index }, .isValid = true, .result = field->IsEnd() };
}


ReportAction ST_Logic::MakeStep(TypeCell cell, size_t index, T_GameField *field)
{
    
}