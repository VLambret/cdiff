#!/usr/bin/env bats

CDIFF=build/cdiff

@test "Given no parameters Cdiff fails" {
	run $CDIFF > /dev/null
	[ $status -ne 0 ]
}

@test "When a single string which is not --help option is passed as argument then program fails" {
	run $CDIFF bad_usage > /dev/null
	[ $status -ne 0 ]
}

@test "Given more than two strings Cdiff fails" {
	run $CDIFF $STRING1 $STRING1 $STRING1 > /dev/null
	[ $status -ne 0 ]
}
