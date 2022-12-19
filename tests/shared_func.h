#include <string>

std::string operator/(std::string const &left, std::string const &right);

GameRoom *CreateRoomById(size_t room_id, std::string const filename = {});

GameRoom *CreateRoom_WithOutput(size_t room_id, std::ostream &out);