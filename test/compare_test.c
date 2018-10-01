#include "unity_src/unity.h"
#include <compare.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

#define END_OF_LINE_PATTERNS {.reference = NULL, .compared = NULL, .expected = NULL}

struct line_patterns {
	char * reference;
	char * compared;
	char * expected;
};

void loop_over_test_cases(struct line_patterns *p) {
	while(p->reference) {
		char result[MAX_LINE_SIZE];
		line_compare(p->reference, p->compared, result);
		TEST_ASSERT_EQUAL_STRING(p->expected, result);
		p++;
	}
}

void test_ThereIsNoDifferenceBetweenTwoIdenticalLines() {
	struct line_patterns givens[] = {
		{.reference = "", .compared = "", .expected = ""},
		{.reference = "toto", .compared = "toto", .expected = "toto"},
		{.reference = "tata", .compared = "tata", .expected = "tata"},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}

void test_AddedSingleCharIsEmphazedWithBrackets() {
	struct line_patterns givens[] = {
		{.reference = "", .compared = "a", .expected = GREEN("a")},
		{.reference = "toto", .compared = "atoto", .expected = GREEN("a")"toto"},
		{.reference = "toto", .compared = "totob", .expected = "toto"GREEN("b")},
		{.reference = "toto", .compared = "tcoto", .expected = "t"GREEN("c")"oto"},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}

int main() {
	UnityBegin("Compare tests");
	RUN_TEST(test_ThereIsNoDifferenceBetweenTwoIdenticalLines);
	RUN_TEST(test_AddedSingleCharIsEmphazedWithBrackets);
	return UnityEnd();
}
