#!/usr/bin/env bats

CDIFF=build/cdiff
TMP_FILE=.cdiff_test.tmp

FILE1=test/samples/helloworld.txt

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
