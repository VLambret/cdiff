#!/usr/bin/env bats

CDIFF=build/cdiff
TMP_STRING=.cdiff_test.tmp

STRING1="toto"
STRING2="tata"

teardown() {
	rm -f $TMP_STRING
}

@test "Given no parameters Cdiff fails" {
	run $CDIFF > /dev/null
	[ $status -ne 0 ]
}

@test "Given a single string Cdiff fails" {
	run $CDIFF $STRING1 > /dev/null
	[ $status -ne 0 ]
}

@test "Given two existing string names Cdiff does not fail" {
	run $CDIFF $STRING1 $STRING1 > /dev/null
	[ $status -eq 0 ]
}

@test "Given more than two strings Cdiff fails" {
	run $CDIFF $STRING1 $STRING1 $STRING1 > /dev/null
	[ $status -ne 0 ]
}

@test "Given two indentical strings then cdiff outputs the exact content of the string" {
	run $CDIFF $STRING1 $STRING1
	[ $status -eq 0 ]
	[ "$output" = "$STRING1" ]
}

@test "Given two indentical strings then cdiff outputs the exact content of the string (Other case)" {
	run $CDIFF $STRING2 $STRING2
	[ $status -eq 0 ]
	[ "$output" = "$STRING2" ]
}
