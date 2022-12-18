#include "room_builder.h"

GameRoom *GameRoomBuilder::build()
{
    T_GameField *field;
    T_GameLogic *logic;
    T_Bot       *bot = nullptr;
    if (_typeGame == TypeGame::ST)
    {
        field 	= new ST_Field;
        logic 	= new ST_Logic;
        if (_withBot)
        {
            bot 	= new ST_Bot;
        }
    }
    else if (_typeGame == TypeGame::OT)
    {
        field 	= new OT_Field;
        logic 	= new OT_Logic;
        if (_withBot)
        {
            bot 	= new OT_Bot;
        }
    }

    Player player_1 = { .id = _player_id_1, .isBot = false,     .cell = TypeCell::X };
    Player player_2 = { .id = _player_id_2, .isBot = _withBot,  .cell = TypeCell::O };

    return new T_Room(_room_id, player_1, player_2, field, logic, _output, bot, _typeGame, _withLoggin);
}

GameRoomBuilder &GameRoomBuilder::withRoomId(size_t room_id)
{
    _room_id = room_id;
    return *this;
}

GameRoomBuilder &GameRoomBuilder::withGame(TypeGame typeGame)
{
    _typeGame = typeGame;
    return *this;
}

// если передан один параметр, подразуемевается, что игра с ботом
GameRoomBuilder &GameRoomBuilder::withPlayers(ssize_t player_id_1, ssize_t player_id_2) 
{
    _player_id_1 = player_id_1;
    _player_id_2 = player_id_2;
    if (_player_id_2 == -1)
    {
        _withBot = true;
    }
    return *this;
}

GameRoomBuilder &GameRoomBuilder::withSizeField(size_t size_field)
{
    _size_field = size_field;
    return *this;
}

GameRoomBuilder &GameRoomBuilder::withOutput(T_Output *output)
{
    _output = output;
    return *this;
}

GameRoomBuilder &GameRoomBuilder::withBot(bool withBot)
{
    _withBot = withBot;
    _player_id_2 = -1;
    return *this;
}
GameRoomBuilder &GameRoomBuilder::withLogging(bool withLoggin)
{
    _withLoggin = withLoggin;
    return *this;
}