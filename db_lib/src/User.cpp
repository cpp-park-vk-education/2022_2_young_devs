#include "User.h"

User::User(const std::string &ip, const std::string &port, const std::string &user,
                       const std::string &password, const std::string &db_name)
{
    database = new DataBase(ip, port, user, password, db_name);
}

User::~User()
{
    database->~DataBase();
}

void User::addUser(const std::string &username, const std::string &login, const std::string &password)
{
    database->Insert("INSERT INTO User(username, login, password) VALUES (?, ?, ?)",
                     {{"S:" + username, "S:" + login, "S:" + password}});
}

UserTable User::getUserInfo(size_t user_id)
{
    auto res = database->Select("SELECT * FROM User WHERE id=?",
                                {"I:" + std::to_string(user_id)});
    return UserTable(res[0]);
}

size_t User::getUserIdLog(const std::string &login)
{
    auto id = database->Select("SELECT id FROM User WHERE login=?",
                               {"S:" + login});
    return std::stoi(id[0][0]);
}

size_t User::getUserIdUsername(const std::string &username)
{
    auto id = database->Select("SELECT id FROM User WHERE username=?",
                               {"S:" + username});
    return std::stoi(id[0][0]);
}

std::string User::getLogin(size_t user_id)
{
    auto log = database->Select("SELECT login FROM User WHERE id=?",
                                 {"I:" + std::to_string(user_id)});
    return log[0][0];
}

std::string User::getUsername(size_t user_id)
{
    auto name = database->Select("SELECT username FROM User WHERE id=?",
                                 {"I:" + std::to_string(user_id)});
    return name[0][0];
}

bool User::userExist(const std::string &login)
{
    auto res = database->Select("SELECT * FROM User WHERE login=?", {"S:" + login});
    if (!res.empty())
    {
        return true;
    }
    return false;
}

bool User::usernameExist(const std::string &username)
{
    auto res = database->Select("SELECT * FROM User WHERE username=?", {"S:" + username});
    if (!res.empty())
    {
        return true;
    }
    return false;
}

bool User::correctAuth(const std::string &login, const std::string &password)
{
    auto res = database->Select("SELECT * FROM User WHERE login=? AND password=?",
                                {"S:" + login, "S:" + password});
    if (!res.empty())
    {
        return true;
    }
    return false;
}

void User::updateUsername(size_t user_id, const std::string &username)
{
    database->Update("UPDATE User SET username=? WHERE id=?",
                     {"S:" + username, "I:" + std::to_string(user_id)});
}

void User::updatePassword(size_t user_id, const std::string &password)
{
    database->Update("UPDATE User SET password=? WHERE id=?",
                     {"S:" + password, "I:" + std::to_string(user_id)});
}

void User::deleteUser(size_t user_id)
{
    database->Delete("DELETE FROM User WHERE id=?", {"I:" + std::to_string(user_id)});
}

std::vector<std::vector<std::string>> User::getTopUsers(size_t top_num)
{
    std::vector<std::pair<std::string, size_t>> top_players;
    auto res = database->Select("SELECT User.id User.username, count(Game.winner_id) AS c "
                                "FROM User JOIN Game ON User.id = Game.winner_id GROUP BY winner_id "
                                "ORDER BY c DESC LIMIT ?", {"I:" + std::to_string(top_num)});
    Game game;
    for (auto & re : res)
    {
        auto num_games = game.userWins(std::stoi(re[0]));
        re.push_back(std::to_string(num_games));
        re.push_back(std::to_string(static_cast<double>(num_games) / std::stoi(re[2])));
    }
    return res;
}
