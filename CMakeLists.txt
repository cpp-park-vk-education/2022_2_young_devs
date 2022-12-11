cmake_minimum_required(VERSION 3.0.0)
project(db)

set(CMAKE_CXX_STANDARD 17)

option(TEST_OPT     "build test version"          OFF)
option(DEBUG_OPT    "build debug version"         ON)
option(SANITIZE_OPT "build with flags -fsanitize" OFF)

add_subdirectory(db_lib)

add_executable(${PROJECT_NAME} main.cpp)

if(TEST_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov")
endif()

if(SANITIZE_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
endif()

link_directories("/usr/lib/mysql")
include_directories("/usr/include/mysql")

target_include_directories(${PROJECT_NAME} PUBLIC ${DB_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DB_LIB_LIBRARIES})

if (TEST_OPT)
    enable_testing()
    add_subdirectory(tests)
endif()
