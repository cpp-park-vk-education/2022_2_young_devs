#include "db.cpp"
#include "User.h"
#include "GameProgress.h"
#include "Game.h"

int main()
{
//    DataBase db("localhost", "3306", "cpp_dev", "123", "cpp_project_db");
//    db.executeQuery("CREATE TABLE IF NOT EXISTS User (id INT NOT NULL AUTO_INCREMENT, name VARCHAR(255) NOT NULL, PRIMARY KEY(id))", {});
//    db.Insert("INSERT INTO User(name) VALUES (?)", {{"S:max"}, {"S:andrey"}, {"S:dsdf"}});
//    db.Delete("DELETE FROM User WHERE id=?", {"I:11"});
//    auto res = db.Select("SELECT ? FROM User WHERE id=?", {"S:name"}, {"I:8"});
//    for (const auto& el : res)
//    {
//        for (const auto& i : el)
//        {
//            std::cout << i << ' ';
//        }
//        std::cout << std::endl;
//    }
//    createTables();
//    insert();
    User user;
    GameProgress game_progress;
    Game game;
    std::cout << game.userWins(1);
//    auto res = game_progress.getPlayerMoves()
    return 0;
}
