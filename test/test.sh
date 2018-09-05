#!/usr/bin/env bats

CDIFF=build/cdiff
TMP_FILE=.cdiff_test.tmp

NON_EXISTING_FILE=CantFindMe
FILE1=test/samples/inputs/helloworld.txt
FILE2=test/samples/inputs/helloworld2.txt
MULTILINE_FILE=test/samples/inputs/multiline.txt

teardown() {
	rm -f $TMP_FILE
}

@test "Given no parameters Cdiff fails" {
	run $CDIFF > /dev/null
	[ $status -ne 0 ]
}

@test "Given a single file Cdiff fails" {
	run $CDIFF $FILE1 > /dev/null
	[ $status -ne 0 ]
}

@test "Given two existing file names Cdiff does not fail" {
	run $CDIFF $FILE1 $FILE1 > /dev/null
	[ $status -eq 0 ]
}

@test "Given more than two files Cdiff fails" {
	run $CDIFF $FILE1 $FILE1 $FILE1 > /dev/null
	[ $status -ne 0 ]
}

@test "Given a non existing file as first parameter diff fails" {
	run $CDIFF $NON_EXISTING_FILE $FILE1 > /dev/null
	[ $status -ne 0 ]
}

@test "Given a non existing file as second parameter diff fails" {
	run $CDIFF $FILE1 $NON_EXISTING_FILE > /dev/null
	[ $status -ne 0 ]
}

@test "Given two indentical files then cdiff outputs the exact content of the file" {
	$CDIFF $FILE1 $FILE1 > $TMP_FILE
	diff $FILE1 $TMP_FILE
}

@test "Given two indentical files then cdiff outputs the exact content of the file (Other case)" {
	$CDIFF $FILE2 $FILE2 > $TMP_FILE
	diff $FILE2 $TMP_FILE
}

@test "Given two indentical multi-line files then outputs the content of the file" {
	$CDIFF $MULTILINE_FILE $MULTILINE_FILE > $TMP_FILE
	diff $MULTILINE_FILE $TMP_FILE
}
