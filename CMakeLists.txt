cmake_minimum_required(VERSION 3.0.0)
project(server)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")

option(TEST_OPT     "build test version"            OFF)
option(DEBUG_OPT    "build debug version"           ON)
option(SANITIZE_OPT "build with flags -fsanitize"   OFF)

set(BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file(config.h.in config.h @ONLY)

# >>>>
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb3 -pthread")

find_package(Boost)

# find_package(Boost 1.71.0 REQUIRED COMPONENTS asio)

# if(NOT Boost_FOUND)
#     message("Not found Boost")
#     return()
# endif()


# для config.h
set(BUILD_DIR "${PROJECT_BINARY_DIR}")


# add_executable(${PROJECT_NAME} main.cpp)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/server/*)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(game_lib)

# add_subdirectory(server)

if(TEST_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

if(DEBUG_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -O0" )
endif()

if(SANITIZE_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${GAME_LIB_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
# target_include_directories(${PROJECT_NAME} PUBLIC ${GAME_LIB_INCLUDE_DIRS})

# ---
target_include_directories(${PROJECT_NAME} PUBLIC "${BUILD_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIB_LIBRARIES} ${Boost_LIBRARY_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIB_LIBRARIES})


if (TEST_OPT)
    enable_testing()
    add_subdirectory(tests)
endif()
