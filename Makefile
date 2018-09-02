.PHONY : all clean test

all : build/cdiff

clean :
	rm -f build/*

test : build/cdiff
	bats test/*.sh


build/cdiff : src/main.c
	gcc -Wall -Werror $^ -o $@
