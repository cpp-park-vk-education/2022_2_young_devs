cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb3 -pthread")

project(boost.asio.client)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*)


add_executable(async.http.client  ${SOURCES})
target_link_libraries(async.http.client  ${Boost_LIBRARIES})
