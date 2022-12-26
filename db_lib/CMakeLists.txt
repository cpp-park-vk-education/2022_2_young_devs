cmake_minimum_required(VERSION 3.0.0)
project(db_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

if(TEST_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fPIC -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )
elseif(DEBUG_OPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O0")
else()
    # релиз-версия
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")
endif()

if(SANITIZE_OPT)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
endif()

file(GLOB_RECURSE   SOURCES       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB           INCLUDE_DIRS  ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE mysqlcppconn)

set(DB_LIB_LIBRARIES       ${PROJECT_NAME}     PARENT_SCOPE)
set(DB_LIB_INCLUDE_DIRS    ${INCLUDE_DIRS}     PARENT_SCOPE)