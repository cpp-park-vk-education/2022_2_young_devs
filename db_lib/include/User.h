#pragma once

#include "DataBase.h"
#include "GameInf.h"


struct UserTable
{
    explicit UserTable(std::vector<std::string> data)
    {
        id = std::stoi(data[0]);
        username = data[1];
        login = data[2];
        password = data[3];
        registration_date = data[4];
    }

    size_t id;
    std::string username;
    std::string login;
    std::string password;
    std::string registration_date;
};

class User
{
private:
    DataBase *database;

public:
    explicit User(const std::string &ip = "localhost", const std::string &port = "3306",
               const std::string &user = "cpp_dev", const std::string &password = "123",
               const std::string &db_name = "cpp_project_db");

    void addUser(const std::string &username, const std::string &login, const std::string &password);
    UserTable getUserInfo(size_t user_id);
    size_t getUserIdLog(const std::string &login);
    size_t getUserIdUsername(const std::string &username);
    std::string getLogin(size_t user_id);
    std::string getUsername(size_t user_id);
    bool userExist(const std::string &login);
    bool usernameExist(const std::string &username);
    bool correctAuth(const std::string &login, const std::string &password);
    void updateUsername(size_t user_id, const std::string &username);
    void updatePassword(size_t user_id, const std::string &password);
    void deleteUser(size_t user_id);
    std::vector<std::vector<std::string>> getTopUsers(size_t top_num);
};