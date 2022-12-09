#include <cassert>

#include "game_field.h"

OT_Field::OT_Field(size_t dim) : _dim(dim), _field(_dim * _dim, TypeCell::E), _sums(_dim * 2 + 2, 0)
{
}


size_t OT_Field::Dimension()
{
    return _dim;
}

size_t OT_Field::Size()
{
    return _dim * _dim;
}

void OT_Field::Clear()
{
    std::fill(_field.begin(), _field.end(), TypeCell::E);
}

TypeCell OT_Field::At(size_t index)
{
    assert(index < _field.size());
	return _field[index];
}

void OT_Field::Set(size_t index, TypeCell cell)
{
    assert(index < _field.size());
	_field[index] = cell;
    CommitStep(index, cell);
}

void OT_Field::Rollback(size_t countSteps, std::vector<StepInfo> steps)
{
    for (size_t i = 0; i < countSteps; ++i)
	{
		StepInfo currentStep = steps.back();
		Set(currentStep.index, TypeCell::E);
		steps.pop_back();
	}
    RecalcSums();
}

void OT_Field::RecalcSums()
{
    _sums.assign(_dim * 2 + 2, TypeCell::E);
	for (size_t i = 0; i < Size(); ++i)
	{
		CommitStep(i, At(i));
	}
}

void OT_Field::CommitStep(size_t index, TypeCell cell)
{
    _sums[index / _dim] += cell;
	_sums[index % _dim + _dim] += cell;

	if (index % _dim == index / _dim)
	{
		_sums[_dim * 2] += cell;
	}
	if (index / _dim == (_dim - 1) - index % _dim)
	{
		_sums[_dim * 2 + 1] += cell;
	}
}
