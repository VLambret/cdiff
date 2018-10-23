CFLAGS=-g --coverage -Isrc
CC=gcc -O0 ${CFLAGS}

COVERAGE_DIR=coverage

.PHONY : all clean test

SRC_FILES=$(filter-out src/main.c, $(wildcard src/*.c))
TEST_FILES=$(wildcard test/unit/test_*.c)
UNITY_FILES=test/unit/unity_src/unity.c

all : build/cdiff

clean : clean_coverage_files
	rm -f build/* *.info *.gcno *.gcda
	rm -rf ${COVERAGE_DIR}
	rm -rf test/unit/unit_main.c
	rm -f profile.txt

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
	bats test/integration/*.bats.sh
	make valgrind

build/unit_tests :  test/unit/unit_main.c ${SRC_FILES} ${TEST_FILES} ${UNITY_FILES}
	${CC} -DUNITY_OUTPUT_COLOR -Itest/unity_src $^ -o $@

test/unit/unit_main.c : test/unit/create_main.sh ${TEST_FILES}
	./$< test/unit/ > $@

################################################################################
# TOOLS
################################################################################

coverage : test
	gcovr -r . --exclude="test*"
	mkdir -p ${COVERAGE_DIR}
	gcovr -r . --exclude="test*" --html --html-details -o ${COVERAGE_DIR}/index.html

valgrind : build/cdiff
	valgrind --leak-check=full build/unit_tests > /dev/null

build/cdiff.prof : src/main.c ${SRC_FILES}
	gcc -Isrc -O3 -pg $^ -o $@

profile.txt : build/cdiff.prof
	$< $$(cat test/samples/bigA.txt) $$(cat test/samples/bigAB.txt)
	gprof $< gmon.out > $@
	rm -f gmon.out

gdb : build/cdiff
	gdb $<
