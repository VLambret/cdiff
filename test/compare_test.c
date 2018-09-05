#include "unity_src/unity.h"
#include <compare.h>

void test_givenTwoIdenticalLines_Then_OutputIsLineContent() {
	char result[MAX_LINE_SIZE];
	line_compare("toto", "toto", result);
	TEST_ASSERT_EQUAL_STRING("toto", result);

}

int main() {
	UnityBegin("Compare tests");
	RUN_TEST(test_givenTwoIdenticalLines_Then_OutputIsLineContent);
	return UnityEnd();
}
