#pragma once

#include "game_field.h"
#include "game_progress.h"
#include <string>

class ICompressor
{
public:
	virtual IRoundStorage *DecompressRound(std::string const &str) = 0;
	virtual std::string CompressRound(IRoundStorage *roundStorage) = 0;
};

class CompressorTicTacToe : public ICompressor
{
public:
	virtual IRoundStorage *DecompressRound(std::string const &str) override;
	virtual std::string CompressRound(IRoundStorage *roundStorage) override;
};
