#! /bin/bash

if [ $# -ne 1 ]
then
	echo "usage : $0 test_dir" 1>&2
	exit
fi

TEST_DIR=$1

head() {
	echo '#include "unity_src/unity.h"'
	echo
}

create_prototypes() {
	TEST_DIR=$1

	for TEST_SUITE in $TEST_DIR/test_*.c
	do
		UNIT_TESTS=$(grep -e '^void test_.*(' $TEST_SUITE | sed -e 's/^void \(test_.*\)(.*/\1/')
		for TEST in $UNIT_TESTS
		do
			echo "void $TEST();"
		done
		echo
	done
}

main() {
	echo 'int main() {'
	echo '	int test_status = 0;'
	echo
}

create_test_suite() {
	TEST_SUITE=$1
	echo "	UnityBegin(\"$(basename $TEST_SUITE)\");"
	UNIT_TESTS=$(grep -e '^void test_.*(' $TEST_SUITE | sed -e 's/^void \(test_.*\)(.*/\1/')
	for TEST in $UNIT_TESTS
	do
		echo "	RUN_TEST($TEST);"
	done
	echo '	if (test_status == 0) {'
	echo '		test_status = UnityEnd();'
	echo '	}'
	echo
}

create_unit_tests() {
	TEST_DIR=$1
	for TEST_SUITE in $TEST_DIR/test_*.c
	do
		create_test_suite $TEST_SUITE
	done
}

end() {
	echo '	return test_status;'
	echo '}'
}

head
create_prototypes $TEST_DIR
main
create_unit_tests $TEST_DIR
end
