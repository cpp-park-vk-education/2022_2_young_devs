#include "gtest/gtest.h"
#include "MySQL_DB.h"

std::vector<std::string> showTables(MYSQL* conn)
{
    MYSQL_RES* res = mysql_list_tables(conn, NULL);
    if (res == NULL)
    {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
    }
    MYSQL_ROW row;
    std::vector<std::string> tables;
    while ((row = mysql_fetch_row(res)))
    {
        tables.push_back(*row);
    }
    mysql_free_result(res);
    return tables;
}

bool operator==(const User& usr1, const User& usr2)
{
    if (usr1.id == usr2.id &&
        usr1.username == usr2.username &&
        usr1.password == usr2.password &&
        usr1.avatar == usr2.avatar &&
        usr1.num_wins == usr2.num_wins &&
        usr1.num_defs == usr2.num_defs &&
        usr1.num_games == usr2.num_games &&
        usr1.winrate == usr2.winrate &&
        usr1.rating == usr2.rating)
    {
        return true;
    }
    return false;
}

TEST(MySQL_DB, testCreateDropTables)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);

    std::vector<std::string> tables;
    tables = showTables(db.getConn());

    EXPECT_EQ(tables.size(), 1);
    EXPECT_EQ(tables[0], "User");

    db.dropTables();

    tables = showTables(db.getConn());

    EXPECT_EQ(tables.size(), 0);
}

TEST(MySQL_DB, testAddDeleteUser)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);
    User user1(std::vector<std::string>{"-1", "user1", "usr1_pass", "avatar1", "0", "0", "0", "0", "0"});
    auto users = db.usersList();

    EXPECT_EQ(users.size(), 0);

    db.addUser(user1);
    users = db.usersList();

    EXPECT_EQ(users.size(), 1);
    EXPECT_EQ(users[0].username, user1.username);
    EXPECT_EQ(users[0].password, user1.password);

    User user2(std::vector<std::string>{"-1", "user2", "usr2_pass", "avatar2", "0", "0", "0", "0", "0"});
    db.addUser(user2);
    users = db.usersList();

    EXPECT_EQ(users.size(), 2);
    EXPECT_EQ(users[1].username, user2.username);
    EXPECT_EQ(users[1].password, "usr2_pass");

    db.deleteUser(user1.username);
    users = db.usersList();

    EXPECT_EQ(users.size(), 1);

    db.deleteUser(user2.username);
    users = db.usersList();

    EXPECT_EQ(users.size(), 0);
}

TEST(MySQL_DB, testUpdateUser)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);
    User user1(std::vector<std::string>{"-1", "user1", "usr1_pass", "avatar1", "0", "0", "0", "0", "0"});
    db.addUser(user1);
    auto users = db.usersList();

    EXPECT_EQ(users.size(), 1);
    EXPECT_EQ(users[0].username, "user1");
    EXPECT_EQ(users[0].password, "usr1_pass");

    user1.username = "1_user";
    user1.password = "1_usr_pass";
    db.updateUser(user1);

    EXPECT_EQ(users.size(), 1);
    EXPECT_EQ(users[0].username, "1_user");
    EXPECT_EQ(users[0].password, "1_usr_pass");
}

TEST(MySQL_DB, testUserProfile)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);
    User user1(std::vector<std::string>{"-1", "user1", "usr1_pass", "avatar1", "0", "0", "0", "0", "0"});
    db.addUser(user1);
    auto user_profile = db.userProfile(user1.username);

    EXPECT_EQ(user_profile.username, "use1");
    EXPECT_TRUE(user_profile == user1);

    int usr_id = db.userID(user_profile.username);

    EXPECT_EQ(usr_id, user_profile.id);
    EXPECT_EQ(db.username(usr_id), user_profile.username);
}

TEST(MySQL_DB, testLeaderBord)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);
    User user1(std::vector<std::string>{"-1", "user1", "usr1_pass", "avatar1", "0", "0", "0", "0", "24"});
    User user2(std::vector<std::string>{"-1", "user2", "usr2_pass", "avatar2", "0", "0", "0", "0", "100"});
    User user3(std::vector<std::string>{"-1", "user3", "usr3_pass", "avatar3", "0", "0", "0", "0", "1324"});
    db.addUser(user1);
    db.addUser(user2);
    db.addUser(user3);
    auto leaders = db.leaderBord();

    EXPECT_EQ(leaders.size(), 3);
    EXPECT_EQ(leaders[0].rating, 1324);
    EXPECT_EQ(leaders[0], user3);
    EXPECT_EQ(leaders[1], user2);
    EXPECT_EQ(leaders[2], user1);
}

TEST(MySQL_DB, testAuth)
{
    MySQL_DB db("localhost", "cpp_dev", "123", "cpp_project_db",0, NULL, 0);
    User user1(std::vector<std::string>{"-1", "user1", "usr1_pass", "avatar1", "0", "0", "0", "0", "24"});
    User user2(std::vector<std::string>{"-1", "user1", "usr2_pass", "avatar2", "0", "0", "0", "0", "100"});
    db.addUser(user1);

    EXPECT_EQ(db.userExist(user1.username), true);
    EXPECT_NE(db.userExist("user2"), true);
    EXPECT_THROW(db.addUser(user2), std::logic_error);

    EXPECT_EQ(db.correctAuth(user1.username, user1.password), true);
    EXPECT_EQ(db.correctAuth("user2", user1.password), false);
    EXPECT_EQ(db.correctAuth(user1.username, "usr2_pass"), false);
}
