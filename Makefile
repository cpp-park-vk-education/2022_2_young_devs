BUILD_DIR 	= build
TESTS_DIR 	= tests
LIB_DIR 	= fib_lib

# project(xxx) on CMakeLists
TESTS_EXE 	= test_fib
TARGET_EXE 	= fib

PATH_LINTERS_SCRIPT = linters/run.sh

TEST_OPT 		= OFF
DEBUG_OPT 		= ON
SANITIZE_OPT 	= OFF

.PHONY: check build test clean run coverage

# Изменения в CMakeLists требует make build
# make build = cmake && cmake --build 
build: clean
	mkdir ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake .. -DTEST_OPT=${TEST_OPT} -DDEBUG_OPT=${DEBUG_OPT} -DSANITIZE_OPT=${SANITIZE_OPT} && $(MAKE) --no-print-directory

clean: 
	(rm -r ${BUILD_DIR} 2>/dev/null) || exit 0 

# инкрементальная сборка и запуск исполняемого файла 
run:
	cd ${BUILD_DIR} && $(MAKE) --no-print-directory
	./${BUILD_DIR}/${TARGET_EXE}

# запуск исполняемого файла с тестированием
test:
	./${BUILD_DIR}/${TESTS_DIR}/${TESTS_EXE}

# проверка исходного кода
check:
	chmod u+x ${PATH_LINTERS_SCRIPT} && ./${PATH_LINTERS_SCRIPT}

coverage:
	cd ${BUILD_DIR} && lcov -t "testing_${LIB_DIR}" -o coverage.info -c -d ${LIB_DIR} && genhtml -o report coverage.info

valgrind_tests:
	valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 ./${BUILD_DIR}/${TESTS_DIR}/${TESTS_EXE}

valgrind_target:
	valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 ./${BUILD_DIR}/${TARGET_EXE}

scan_build:
	cd ${BUILD_DIR} && scan-build $(MAKE) --no-print-directory

formating:
	clang-format -i --style=Microsoft  main.cpp ${LIB_DIR}/include/*.h ${LIB_DIR}/src/*.cpp ${TESTS_DIR}/*.cpp

