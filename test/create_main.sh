#! /bin/bash

if [ $# -ne 1 ]
then
	echo "usage : $0 test_dir" 1>&2
	exit
fi

TEST_DIR=$1

list_unit_tests() {
	for TEST_SUITE in $TEST_DIR/test_*.c
	do
		grep -e '^void test_.*(' $TEST_SUITE | sed -e 's/^void \(test_.*\)(.*/\1/'
	done
}

UNIT_TESTS=$(list_unit_tests)

echo '#include "unity_src/unity.h"'
echo
for TEST in $UNIT_TESTS
do
	echo "void $TEST();"
done
echo

echo 'int main() {'
echo '	UnityBegin("cdiff tests");'
for TEST in $UNIT_TESTS
do
	echo "	RUN_TEST($TEST);"
done
echo '	return UnityEnd();'
echo '}'
