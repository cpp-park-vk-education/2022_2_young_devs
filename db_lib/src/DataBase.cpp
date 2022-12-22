#include "DataBase.h"


DataBase::DataBase()
{
    try {
        driver = get_driver_instance();
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
}

DataBase::DataBase(const std::string &ip, const std::string &port, const std::string &user,
                   const std::string &password, const std::string &db_name)
{
    try {
        driver = get_driver_instance();
        Connect(ip, port, user, password);
        changeSchema(db_name);
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
}

DataBase::~DataBase()
{
    delete res;
//    delete stmt;
    delete prep_stmt;
    delete con;
}

void DataBase::Connect(const std::string &ip, const std::string &port, const std::string &user,
                       const std::string &password)
{
    try {
        con = driver->connect(ip + ":" + port, user, password);
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
}

void DataBase::changeSchema(const std::string &db_name)
{
    try {
        con->setSchema(db_name);
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
}

void DataBase::executeQuery(const std::string &query, const std::vector<std::string> &params)
{
    try {
        prep_stmt = con->prepareStatement(query);
        for (size_t i = 0; i < params.size(); i++)
        {
            if (params[i].substr(0, 2) == "S:")
            {
                prep_stmt->setString(i + 1, params[i].substr(2, params[i].size()));
            }
            else if (params[i].substr(0, 2) == "I:")
            {
                prep_stmt->setInt(i + 1, std::stoi(params[i].substr(2, params[i].size())));
            }
        }
        prep_stmt->execute();
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
}

void DataBase::Insert(const std::string &query, const std::vector<std::vector<std::string>> &params)
{
    for (auto & param : params)
    {
        executeQuery(query, param);
    }
}

void DataBase::Update(const std::string &query, const std::vector<std::string> &params)
{
    executeQuery(query, params);
}

void DataBase::Delete(const std::string &query, const std::vector<std::string> &params)
{
    executeQuery(query, params);
}

std::vector<std::vector<std::string>>
DataBase::Select(const std::string &query, const std::vector<std::string> &params)
{
    std::vector<std::string> row;
    std::vector<std::vector<std::string>> query_res;
    executeQuery(query, params);
    try {
        res = prep_stmt->getResultSet();
        sql::ResultSetMetaData *res_meta = res->getMetaData();
        while (res->next())
        {
            for (size_t i = 1; i <= res_meta->getColumnCount(); i++)
            {
                row.push_back(res->getString(i));
            }
            query_res.push_back(row);
            row.clear();
        }
    }
    catch (sql::SQLException &e)
    {
        std::cout << "# ERR: " << e.what();
    }
    return query_res;
}
