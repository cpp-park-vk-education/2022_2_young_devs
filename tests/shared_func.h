#include <string>

std::string operator/(std::string const &left, std::string const &right);

GameRoom *CreateRoomById(size_t room_id, std::string const filename = {});

GameRoom *CreateRoom_WithOutput(size_t room_id, std::ostream &out);

std::optional<GameRoom *> _findRoomById(std::vector<GameRoom *> &items, size_t id);

void showTime(auto start, std::string &&message);