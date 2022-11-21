#include "MySQL_DB.h"

int main()
{
    std::cout << "MySQL client version: " << mysql_get_client_info() << std::endl;
    exit(0);
}



