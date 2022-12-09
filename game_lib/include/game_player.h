#pragma once

#include <cstddef>

class Player
{
private:
    size_t _player_id;
public:
    size_t Id();
};

class T_Player : public Player
{
public:
    virtual bool IsBot() = 0;
};

class Bot : public T_Player
{
public:
    virtual bool IsBot() override;
};


class RealPlayer : public T_Player
{
public:
    virtual bool IsBot() override;
};