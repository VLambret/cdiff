#include "unity_src/unity.h"
#include <compare.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

#define END_OF_LINE_PATTERNS {.reference = NULL, .compared = NULL, .expected = NULL}

#define MAX_LINE_SIZE 4096

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

void test_GivenIdenticalLines_Then_OutputIsACopyOfReferenceLine() {
	struct line_patterns givens[] = {
		{.reference = "", .compared = "", .expected = ""},
		{.reference = "a", .compared = "a", .expected = "a"},
		{.reference = "ab", .compared = "ab", .expected = "ab"},
		{.reference = "abcd", .compared = "abcd", .expected = "abcd"},
		{.reference = "vim>emacs", .compared = "vim>emacs", .expected = "vim>emacs"},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}

void test_GivenComparedLineWithMoreChars_The_AdditionalCharactersAreEmphazedWithGreen() {
	struct line_patterns givens[] = {
		{.reference = "", .compared = "a", .expected = GREEN("a")},
		{.reference = "a", .compared = "ab", .expected = "a"GREEN("b")},
		{.reference = "b", .compared = "ab", .expected = GREEN("a")"b"},
		{.reference = "abc", .compared = "abcde", .expected = "abc"GREEN("de")},
		{.reference = "ae", .compared = "abcde", .expected = "a"GREEN("bcd")"e"},
		{.reference = "cde", .compared = "abcde", .expected =GREEN("ab") "cde"},
		{.reference = "movie is really bad !",
		 .compared  = "lantern movie is really bad !",
		 .expected  = ""GREEN("lantern ")"movie is really bad !"},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}

void test_GivenComparedLineWithMissingChars_The_RemovedCharactersAreEmphazedWithRed() {
	struct line_patterns givens[] = {
		{.reference = "a", .compared = "", .expected = RED("a")},
		{.reference = "ab", .compared = "a", .expected = "a"RED("b")},
		{.reference = "ab", .compared = "b", .expected = RED("a")"b"},
		{.reference = "abcde", .compared = "abc", .expected = "abc"RED("de")},
		{.reference = "abcde", .compared = "ae", .expected = "a"RED("bcd")"e"},
		{.reference = "abcde", .compared = "cde", .expected =RED("ab") "cde"},
		{.reference = "no hood movie at the moment...",
		 .compared  = "no movie at the moment...",
		 .expected  = "no "RED("hood ")"movie at the moment..."},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}

void test_SubstitutionAreMarkedAsTheResultOfARemovalAndAnAdding() {
	struct line_patterns givens[] = {
		{.reference = "a", .compared = "b", .expected = RED("a")GREEN("b")},
		{.reference = "ab", .compared = "ac", .expected = "a"RED("b")GREEN("c")},
		{.reference = "ab", .compared = "cb", .expected =RED("a")GREEN("c")"b"},
		{.reference = "abc", .compared = "adc", .expected ="a"RED("b")GREEN("d")"c"},

		{.reference = "ital", .compared = "alie", .expected = RED("it")"al"GREEN("ie")},
		{.reference = "itie", .compared = "alie", .expected =RED("it")GREEN("al")"ie"},

		{.reference = "ital", .compared = "itie", .expected = "it"RED("al")GREEN("ie")},
		{.reference = "itie", .compared = "ital", .expected = "it"RED("ie")GREEN("al")},

		{.reference = "alie", .compared = "itie", .expected = RED("al")GREEN("it")"ie"},
		// XXX : This specific case is not handled correctly at the moment
		//{.reference = "alie", .compared = "ital", .expected = GREEN("it")"al"RED("ie")},
		END_OF_LINE_PATTERNS
	};
	loop_over_test_cases(givens);
}
