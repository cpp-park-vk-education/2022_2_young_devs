#include "compressor.h"
#include <cmath>

IGameField *ManagerTicTacToeField::DecompressField(std::string const &str)
{
    size_t size = (str[0] - 48) * 10 + (str[1] - 48);
    IGameField *field = new TicTacToeField(std::sqrt(size));
    for (size_t i = 0; i < size; ++i)
    {
        (*field)[i] = str[i + 2] - 48;
    }
    return field;
}
std::string ManagerTicTacToeField::CompressField(IGameField *field)
{
    size_t size = field->Size() * field->Size();
    std::string strCompressField(size + 2, 0);
    strCompressField[0] = (size / 10) + 48;
    strCompressField[1] = (size % 10) + 48;
    for (size_t i = 0; i < size; ++i)
    {
        strCompressField[i + 2] = (*field)[i] + 48;
    }
    return strCompressField;
}
