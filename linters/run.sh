#!/usr/bin/env bash

set -o pipefail

SRC_PATHS="main.cpp fib_lib/src/*.cpp"
INCLUDE_PATHS="fib_lib/include/*.h"
TESTS_PATHS="tests/*.cpp"

INCLUDE_DIRECTORIES="fib_lib/include"

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

# ********** cppcheck **********
print_header "RUN cppcheck"
check_log "cppcheck ${SRC_PATHS} ${INCLUDE_PATHS} ${TESTS_PATHS} --enable=all --inconclusive --error-exitcode=1 -I${INCLUDE_DIRECTORIES} --suppress=missingIncludeSystem" "\(information\)"

# # ********** clang-tidy **********
print_header "RUN clang-tidy"
check_log "clang-tidy ${SRC_PATHS} ${TESTS_PATHS} -warnings-as-errors=* -extra-arg=-std=c++17 -- -I${INCLUDE_DIRECTORIES} -x c++" "Error (?:reading|while processing)"


# # ********** cpplint **********
print_header "RUN cpplint"
check_log "cpplint --extensions=cpp ${SRC_PATHS} ${TESTS_PATHS}"    "Can't open for reading"
check_log "cpplint --extensions=h   ${INCLUDE_PATHS}"               "Can't open for reading"


# # ********** clang-format **********
print_header "RUN clang-format"
diff <(clang-format --style=Microsoft ${SRC_PATHS} ${INCLUDE_PATHS} ${TESTS_PATHS}) <(cat ${SRC_PATHS} ${INCLUDE_PATHS} ${TESTS_PATHS}) || exit 1

print_header "SUCCESS"