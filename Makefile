.PHONY : all clean test

all : build/cdiff

clean :
	rm -f build/*

test : build/cdiff build/unit_tests
	bats test/*.sh
	./build/unit_tests

build/unit_tests : src/compare.c test/compare_test.c test/unity_src/unity.c
	gcc -g -DUNITY_OUTPUT_COLOR -Isrc -Itest/unity_src $^ -o $@

build/cdiff : src/main.c
	gcc -g -Wall -Werror $^ -o $@

src/compare.c : src/compare.h
