#!/usr/bin/env bats

CDIFF=build/cdiff
TMP_FILE=.cdiff_test.tmp

teardown() {
    rm -f $TMP_FILE
}

@test "Given no parameters Cdiff should fail" {
    run $CDIFF > /dev/null
    [ $status -ne 0 ]
}


