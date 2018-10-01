CFLAGS=-g --coverage -Isrc
CC=gcc -O0 ${CFLAGS}

COVERAGE_DIR=coverage

.PHONY : all clean test

all : build/cdiff

clean : clean_coverage_files
	rm -f build/* *.info *.gcno *.gcda
	rm -rf ${COVERAGE_DIR}

clean_coverage_files :
	rm -f $$(find -name '*.gcno' -o -name '*.gcda')

test : build/cdiff build/unit_tests
	bats test/*.sh
	./build/unit_tests

build/unit_tests : src/compare.c test/compare_test.c test/unity_src/unity.c
	${CC} -DUNITY_OUTPUT_COLOR -Itest/unity_src $^ -o $@

build/cdiff : src/main.c src/compare.c
	${CC} -Wall -Werror $^ -o $@

src/compare.c : src/compare.h

coverage : test
	gcovr -r . --exclude="test*"
	mkdir -p ${COVERAGE_DIR}
	gcovr -r . --exclude="test*" --html --html-details -o ${COVERAGE_DIR}/index.html
