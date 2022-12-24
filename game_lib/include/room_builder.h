#include "game_room.h"

class GameRoomBuilder
{
public:
	T_Room *build();

    GameRoomBuilder &withRoomId(size_t room_id);

	GameRoomBuilder &withGame(TypeGame typeGame);

    // если передан один параметр, подразуемевается, что игра с ботом
	GameRoomBuilder &withPlayers(ssize_t player_id_1, ssize_t player_id_2 = -1);

	GameRoomBuilder &withSizeField(size_t size_field);

	GameRoomBuilder &withOutput(T_Output *output);

	GameRoomBuilder &withBot(bool withBot);

    GameRoomBuilder &withLogging(bool withLoggin);

private:
    size_t      _room_id = 0;
	TypeGame    _typeGame = TypeGame::OT;
    ssize_t      _player_id_1;
    ssize_t      _player_id_2;
    size_t      _size_field = 3;
    T_Output    *_output;
    bool        _withBot = false;
    bool        _withLoggin = false;
};