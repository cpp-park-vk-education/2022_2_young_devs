#include "game_player.h"


size_t Player::Id()
{
    return _player_id;
};

bool Bot::IsBot() 
{
    return true;
}

bool RealPlayer::IsBot()
{
    return false;   
}
