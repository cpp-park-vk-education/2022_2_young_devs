#pragma once

#include <cstdint>
#include <vector>

enum CELL
{
	X = 1,
	E = 0,
	O = -1
};

class IGameField
{
public:
	virtual typename std::vector<int16_t>::iterator Begin() = 0;
	virtual typename std::vector<int16_t>::iterator End() = 0;

	virtual size_t Size() const = 0;

	virtual void Clear() = 0;

	virtual void SetField(IGameField *field) = 0;

	virtual int16_t operator[](size_t index) const = 0;
	virtual int16_t &operator[](size_t index) = 0;
};

class TicTacToeField : public IGameField
{
public:
	TicTacToeField(size_t n = 3);

	virtual int16_t operator[](size_t index) const override;
	virtual int16_t &operator[](size_t index) override;

	virtual size_t Size() const override;

	virtual typename std::vector<int16_t>::iterator Begin() override;
	virtual typename std::vector<int16_t>::iterator End() override;

	virtual void Clear() override;

	// копирует данные, указателем не владеет
	virtual void SetField(IGameField *field) override;

private:
	size_t _n;
	std::vector<int16_t> _field;
};

class GameTestField : public IGameField
{
public:
	GameTestField(size_t n = 3);
	virtual typename std::vector<int16_t>::iterator Begin() override;
	virtual typename std::vector<int16_t>::iterator End() override;

	virtual size_t Size() const override;

	virtual int16_t operator[](size_t index) const override;
	virtual int16_t &operator[](size_t index) override;

	virtual void Clear() override;

	// копирует данные, указателем не владеет
	virtual void SetField(IGameField *field) override;

private:
	size_t _n;
	std::vector<int16_t> _testField;
};
