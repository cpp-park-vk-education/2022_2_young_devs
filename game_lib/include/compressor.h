#pragma once

#include "game_field.h"
#include "game_progress.h"
#include <string>

class ICompressor
{
public:
	virtual IGameProgress *DecompressField(std::string const &str) = 0;
	virtual std::string CompressField(IGameProgress *field) = 0;
};

class CompressorTicTacToe : public ICompressor
{
public:
	virtual IGameProgress *DecompressField(std::string const &str) override;
	virtual std::string CompressField(IGameProgress *progress) override;
};
