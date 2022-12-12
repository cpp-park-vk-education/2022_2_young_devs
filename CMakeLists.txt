cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp
        Game/Game.cpp
        GameWidget/GameWidget.cpp
        GameField/GameField.cpp
        RulesWidget/RulesWidget.cpp
        ScoresWidget/ScoresWidget.cpp)


find_package(Threads REQUIRED)
find_package(Wt REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

option(TEST_OPT     "build test version"            OFF)
option(DEBUG_OPT    "build debug version"           ON)
option(SANITIZE_OPT "build with flags -fsanitize"   OFF)

set(BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file(config.h.in config.h @ONLY)

add_subdirectory(game_lib)

if(TEST_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
endif()

if(DEBUG_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -fPIC -O0" )
endif()

if(SANITIZE_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
endif()

# target_include_directories(${PROJECT_NAME} PUBLIC ${GAME_LIB_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${GAME_LIB_INCLUDE_DIRS})

# для config.h
set(BUILD_DIR "${PROJECT_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${BUILD_DIR}")

# target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIB_LIBRARIES} ${Boost_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIB_LIBRARIES} -ldl Wt::Wt Wt::HTTP)

if (TEST_OPT)
    enable_testing()
    add_subdirectory(tests)
endif()
