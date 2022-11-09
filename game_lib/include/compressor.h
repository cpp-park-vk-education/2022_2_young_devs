#pragma once

#include "game_field.h"
#include <string>

class IManagerField
{
  public:
    virtual IGameField *DecompressField(std::string const &str) = 0;
    virtual std::string CompressField(IGameField *field) = 0;
};

class ManagerTicTacToeField : public IManagerField
{
  public:
    virtual IGameField *DecompressField(std::string const &str) override;
    virtual std::string CompressField(IGameField *field) override;
};
