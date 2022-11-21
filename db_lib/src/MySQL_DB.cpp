#include "MySQL_DB.h"

MySQL_DB::MySQL_DB(const std::string &host, const std::string &user, const std::string &passwd, const std::string &db,
                   const size_t &port, const std::string &unix_socket, const size_t &client_flag)
                   : conn(mysql_init(NULL))
{
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "could not initialize MySQL client library\n");
        exit(1);
    }
    if(!(conn))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(conn));
        exit(1);
    }
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(),
                            port, unix_socket.c_str(), client_flag))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    createTables();
}

MySQL_DB::~MySQL_DB()
{
    mysql_close(conn);
    mysql_library_end();
}

MYSQL *MySQL_DB::getConn() const {
    return conn;
}




