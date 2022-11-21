#pragma once
#include "DataBase.h"
#include <mysql.h>
#include "Tables.h"

class MySQL_DB : public DataBase
{
private:
    MYSQL* conn;

    virtual void create(const std::string& table, const std::vector<std::string>& arg) override;
    virtual void insert(const std::string& table, const std::vector<std::string>& values) override;
    virtual void update(const std::string& table, const std::vector<std::string>& set, const std::string& where = "") override;
    virtual void delete_(const std::string& table, const std::string& where = "") override;
    virtual std::vector<std::string> select(const std::string& table, const std::vector<std::string>& what = std::vector<std::string>(), const std::string& param = "") override;
    virtual void drop(const std::string& table, const std::vector<std::string>& arg) override;
public:
    MySQL_DB(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db,
             const size_t& port = 0, const std::string& unix_socket = NULL, const size_t& client_flag = 0);
    ~MySQL_DB();
    void createTables();
    void dropTables();
    void addUser(User user);
    int userID(const std::string& username);
    std::string username(int id);
    User userProfile(const std::string& username);
    std::vector<User> usersList();
    std::vector<User> leaderBord();
//    std::vector<User> lookingForGameUsers();
    bool userExist(const std::string& username);
    bool correctAuth(const std::string& username, const std::string& password);
    void updateUser(User user);
    void deleteUser(const std::string& username);
    MYSQL* getConn() const;
};
