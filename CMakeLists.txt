cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 14)

find_package(Wt REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        Game/Game.cpp
        GameWidget/GameWidget.cpp
        GameField/GameField.cpp
        RulesWidget/RulesWidget.cpp
        ScoresWidget/ScoresWidget.cpp)

target_link_libraries(${PROJECT_NAME} -ldl Wt::Wt Wt::HTTP)

