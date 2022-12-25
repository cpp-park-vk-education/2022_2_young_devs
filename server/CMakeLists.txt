cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -Wall -ggdb3 -pthread")

project(server)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS} ${GAME_LIB_INCLUDE_DIRS})

# Multithread Async HTTP Server
add_executable(async.http.server connection.h connection.cpp request.h response.h router.h async.http.server.h async.http.server.cpp)

target_link_libraries(async.http.server  ${Boost_LIBRARIES} ${GAME_LIB_INCLUDE_DIRS})