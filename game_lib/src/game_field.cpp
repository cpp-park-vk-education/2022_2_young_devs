#include <cassert>
#include <algorithm>

#include "game_field.h"
#include "helpers_func.h"

using namespace std::string_literals;


OT_Field::OT_Field(size_t dim) : _dim(dim), _field(_dim * _dim, TypeCell::E), _sums(_dim * 2 + 2, 0)
{
}


size_t OT_Field::Dimension()
{
    return _dim;
}

size_t OT_Field::Size() const
{
    return _dim * _dim;
}

void OT_Field::Clear()
{
	std::lock_guard locker(_mutex);
    std::fill(_field.begin(), _field.end(), TypeCell::E);
}

TypeCell OT_Field::At(size_t index) const
{
    assert(index < Size());
	std::lock_guard locker(_mutex);
	return _field[index];
}

void OT_Field::Set(size_t index, TypeCell cell)
{
    assert(index < Size());
	std::lock_guard locker(_mutex);
	_field[index] = cell;
    CommitStep(index, cell);
}

void OT_Field::Rollback(size_t countSteps, std::vector<StepInfo> &steps)
{
	std::lock_guard locker(_mutex);
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
	assert(index < Size());
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

GameResult OT_Field::IsEnd()
{
	std::lock_guard locker(_mutex);
	GameResult result;
	for (size_t i = 0; i < _sums.size(); ++i)
	{
		if (_sums[i] == _dim)
		{
			result.isEnd = true;
			result.winnerCell = TypeCell::X;
			return result;
		}
		if (_sums[i] == -_dim)
		{
			result.isEnd = true;
			result.winnerCell = TypeCell::O;
			return result;
		}
	}
	auto it = std::find(_field.begin(), _field.end(), TypeCell::E);
	if (it == _field.end())
	{
		result.isEnd = true;
		result.draw = true;
		result.winnerCell = TypeCell::E;
		return result;
	}
	result.isEnd = false;
	return result;
}

// ************************************************************************

ST_Field::ST_Field() : _dim(9), _dim_fields(3), _miniature(3),
	_field({
		OT_Field(3), OT_Field(3), 
		OT_Field(3), OT_Field(3), 
		OT_Field(3), OT_Field(3), 
		OT_Field(3), OT_Field(3), 
		OT_Field(3)
	})
{
}

size_t ST_Field::Dimension()
{
    return _dim;
}

size_t ST_Field::Size() const
{
    return _dim * _dim;
}

void ST_Field::Clear()
{
	std::lock_guard locker(_mutex);
    std::for_each(_field.begin(), _field.end(), [](OT_Field &field){
		field.Clear();
	});
}

TypeCell ST_Field::At(size_t index) const
{
    assert(index < Size());
	return _field[index / _dim].At(index % _dim);
}

void ST_Field::Set(size_t index, TypeCell cell)
{
    assert(index < Size());
	std::lock_guard locker(_mutex);
	_field[index / _dim].Set(index % _dim, cell);
    CommitStep(index, cell);
}

void ST_Field::Rollback(size_t countSteps, std::vector<StepInfo> &steps)
{
	std::lock_guard locker(_mutex);
    for (size_t i = 0; i < countSteps; ++i)
	{
		StepInfo currentStep = steps.back();
		Set(currentStep.index, TypeCell::E);
		steps.pop_back();
	}
}

void ST_Field::CommitStep(size_t index, TypeCell cell)
{
	assert(index < Size());
	if (_field[index / _dim].IsEnd().isEnd && _miniature.At(index / _dim) == TypeCell::E)
	{
		TypeCell cell = _field[index / _dim].IsEnd().winnerCell;
		_miniature.Set(index / _dim, cell);
	}
}

GameResult ST_Field::IsEnd()
{
	return _miniature.IsEnd();
}


// DEBUG

// void show(std::vector<TypeCell> vec, std::string message = {})
// {
// 	std::cout << message << std::endl;
//     for (size_t i = 0; i < vec.size(); ++i)
//     {
//         std::string strCell = TypeCellStr(vec[i]);
// 		std::cout << strCell << ' ';
// 		if (i % 3 == 3 - 1)
// 		{
// 			std::cout << '\n';
// 		}
//     }
// }

// void showsums(std::vector<int> vec)
// {
// 	std::cout << "SUMS_____________" << std::endl;
// 	for (size_t i = 0; i < vec.size(); ++i)
// 	{
// 		std::cout << vec[i] << " ";
// 	}
// 	std::cout << "\n";
// }