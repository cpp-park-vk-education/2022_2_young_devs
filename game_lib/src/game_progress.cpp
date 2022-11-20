#include "game_progress.h"

TicTacToeProgress::TicTacToeProgress(size_t sizeField) : _sizeField(sizeField)
{
}

std::vector<Step> TicTacToeProgress::GetSteps() const
{
	return _steps;
}

void TicTacToeProgress::AddStep(size_t indexCell, size_t indexPlayer, int16_t valueCell)
{
	_steps.push_back({.indexCell = indexCell, .indexPlayer = indexPlayer, .valueCell = valueCell});
}

void TicTacToeProgress::Rollback(IGameField *field, size_t countSteps)
{
	for (size_t i = 0; i < countSteps; ++i)
	{
		Step currentStep = _steps.back();
		field->ChangeField(currentStep.indexCell, E);
		_steps.pop_back();
	}
}

void TicTacToeProgress::AddWinner(size_t indexPlayer)
{
	_indexWinner = indexPlayer;
}

void TicTacToeProgress::InitializeField(IGameField *field) const
{
	for (size_t i = 0; i < _steps.size(); ++i)
	{
		field->ChangeField(_steps[i].indexCell, _steps[i].valueCell);
	}
}