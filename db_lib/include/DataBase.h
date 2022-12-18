#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DataBase
{
private:
    friend class User;
    friend class GameInf;
    friend class GameProgress;

    sql::Driver *driver;
    sql::Connection *con;
    sql::ResultSet *res;
    sql::PreparedStatement *prep_stmt;

    void Insert(const std::string &query, const std::vector<std::vector<std::string>>& params);
    void Update(const std::string &query, const std::vector<std::string> &params);
    void Delete(const std::string &query, const std::vector<std::string> &params);
    std::vector<std::vector<std::string>>
    Select(const std::string &query, const std::vector<std::string> &params = {});

    void executeQuery(const std::string &query, const std::vector<std::string> &params);

    DataBase();
    DataBase(const std::string &ip, const std::string &port, const std::string &user,
             const std::string &password, const std::string &db_name);
    ~DataBase();

    void Connect(const std::string &ip, const std::string &port, const std::string &user, const std::string &password);
    void changeSchema(const std::string &db_name);
};
