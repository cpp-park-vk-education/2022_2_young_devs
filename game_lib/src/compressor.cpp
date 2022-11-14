#include "compressor.h"
#include <cmath>

// разница между цифрами и их кодами ANSII
constexpr uint8_t DIFF_ANSII = 48;
// количество символов, отводимое под размер поля в стркое
constexpr uint8_t LEN_SIZE = 1;
// количество полей в структуре Step (индекс ячейки, индекс игрока, значение ячейки)
constexpr uint8_t COUNT_FIELDS = 3;

IRoundStorage *CompressorTicTacToe::DecompressRound(std::string const &strCompressed)
{
	size_t sizeField = strCompressed[0] - DIFF_ANSII;
	IRoundStorage *roundStorage = new TicTacToeProgress(sizeField);
	size_t countSteps = (strCompressed.size() - LEN_SIZE) / COUNT_FIELDS;
	for (size_t i = 0; i < countSteps; ++i)
	{
		size_t indexCell = strCompressed[i * COUNT_FIELDS + 0 + LEN_SIZE] - DIFF_ANSII;
		size_t indexPlayer = strCompressed[i * COUNT_FIELDS + 1 + LEN_SIZE] - DIFF_ANSII;
		int16_t valueCell = strCompressed[i * COUNT_FIELDS + 2 + LEN_SIZE] - DIFF_ANSII;
		roundStorage->AddStep(indexCell, indexPlayer, valueCell);
	}
	return roundStorage;
}

std::string CompressorTicTacToe::CompressRound(IRoundStorage *roundStorage)
{
	std::vector<Step> steps = roundStorage->GetSteps();

	std::string strCompressed;

	size_t sizeField = roundStorage->GetSizeField();
	strCompressed += char(sizeField + DIFF_ANSII);
	for (size_t i = 0; i < steps.size(); ++i)
	{
		// где кто что
		strCompressed += char(steps[i].indexCell + DIFF_ANSII);
		strCompressed += char(steps[i].indexPlayer + DIFF_ANSII);
		strCompressed += char(steps[i].valueCell + DIFF_ANSII);
	}
	return strCompressed;
}
