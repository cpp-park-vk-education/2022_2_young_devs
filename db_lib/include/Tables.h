#pragma once
#include <string>
#include <vector>

struct User
{
    int id;
    std::string username;
    std::string password;
    std::string avatar;
    int num_wins;
    int num_defs;
    int num_games;
    double winrate;
    int rating;
//    std::string status;
    User();
    User(std::vector<std::string> val);
};
