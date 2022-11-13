#include "compressor.h"
#include <cmath>

// разница между цифрами и их кодами ANSII
constexpr uint8_t DIFF_ANSII = 48;
// количество символов, отводимое под размер поля в стркое
constexpr uint8_t LEN_SIZE = 1;
// количество полей в структуре Step (индекс ячейки, индекс игрока, значение ячейки)
constexpr uint8_t COUNT_FIELDS = 3;

IGameProgress *CompressorTicTacToe::DecompressField(std::string const &strCompressed)
{
	size_t sizeField = strCompressed[0] - DIFF_ANSII;
	IGameProgress *progress = new TicTacToeProgress(sizeField);
	size_t countSteps = (strCompressed.size() - LEN_SIZE) / COUNT_FIELDS;
	for (size_t i = 0; i < countSteps; ++i)
	{
		size_t indexCell = strCompressed[i * COUNT_FIELDS + 0 + LEN_SIZE] - DIFF_ANSII;
		size_t indexPlayer = strCompressed[i * COUNT_FIELDS + 1 + LEN_SIZE] - DIFF_ANSII;
		int16_t valueCell = strCompressed[i * COUNT_FIELDS + 2 + LEN_SIZE] - DIFF_ANSII;
		progress->AddStep(indexCell, indexPlayer, valueCell);
	}
	return progress;
}

std::string CompressorTicTacToe::CompressField(IGameProgress *progress)
{
	std::vector<Step> steps = progress->GetSteps();

	std::string strCompressed;

	size_t sizeField = progress->GetSizeField();
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
