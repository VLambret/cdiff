CFLAGS=-g --coverage -Isrc
CC=gcc -O0 ${CFLAGS}

COVERAGE_DIR=coverage

.PHONY : all clean test

SRC_FILES=$(filter-out src/main.c, $(wildcard src/*.c))
TEST_FILES=$(wildcard test/test_*.c)
UNITY_FILES=test/unity_src/unity.c

all : build/cdiff

clean : clean_coverage_files
	rm -f build/* *.info *.gcno *.gcda
	rm -rf ${COVERAGE_DIR}
	rm -rf test/unit_test_main.c

clean_coverage_files :
	rm -f $$(find -name '*.gcno' -o -name '*.gcda')

################################################################################
# BUILD
################################################################################

build/cdiff : src/main.c ${SRC_FILES}
	${CC} -Wall -Werror $^ -o $@

src/compare.c : src/compare.h

src/modification.c : src/modification.h

################################################################################
# TEST
################################################################################

test : build/cdiff build/unit_tests
	./build/unit_tests
	bats test/test.sh
	make valgrind

build/unit_tests :  test/unit_test_main.c ${SRC_FILES} ${TEST_FILES} ${UNITY_FILES}
	${CC} -DUNITY_OUTPUT_COLOR -Itest/unity_src $^ -o $@

test/unit_test_main.c : test/create_main.sh ${TEST_FILES}
	./$< test > $@

coverage : test
	gcovr -r . --exclude="test*"
	mkdir -p ${COVERAGE_DIR}
	gcovr -r . --exclude="test*" --html --html-details -o ${COVERAGE_DIR}/index.html

valgrind : build/cdiff
	valgrind --leak-check=full build/unit_tests > /dev/null

gdb : build/cdiff
	gdb $<
