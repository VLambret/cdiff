#include "unity_src/unity.h"
#include <modification.h>
#include <string.h>
#include <stdbool.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

// String conversion

void test_modification_string_modification_with_single_text() {
	struct modification m = {TEXT, "abcd", 4, NULL};
	char result[1024];
	modification_to_string(&m, result);
	TEST_ASSERT_EQUAL_STRING("abcd", result);
}

void test_modification_string_modification_with_only_added_text() {
	struct modification text_added = {ADDING, "a", 1, NULL};
	char result[1024];
	modification_to_string(&text_added, result);
	TEST_ASSERT_EQUAL_STRING(GREEN("a"), result);
}

void test_modification_string_modification_with_only_removed_text() {
	struct modification text_removed = {REMOVAL, "a", 1, NULL};
	char result[1024];
	modification_to_string(&text_removed, result);
	TEST_ASSERT_EQUAL_STRING(RED("a"), result);
}

void test_modification_string_modification_with_added_text_at_the_beginning() {
	struct modification suffix = {TEXT, "bcde", 4, NULL};
	struct modification text_added = {ADDING, "a", 1, &suffix};
	char result[1024];
	modification_to_string(&text_added, result);
	TEST_ASSERT_EQUAL_STRING(GREEN("a")"bcde", result);
}

void test_modification_string_modification_with_added_text_in_the_middle() {
	struct modification suffix = {TEXT, "de", 2, NULL};
	struct modification text_added = {ADDING, "c", 1, &suffix};
	struct modification prefix = {TEXT, "ab", 2, &text_added};
	char result[1024];
	modification_to_string(&prefix, result);
	TEST_ASSERT_EQUAL_STRING("ab"GREEN("c")"de", result);
}

void test_modification_string_modification_with_added_text_at_the_end() {
	struct modification text_added = {ADDING, "e", 1, NULL};
	struct modification base = {TEXT, "abcd", 4, &text_added};
	char result[1024];
	modification_to_string(&base, result);
	TEST_ASSERT_EQUAL_STRING("abcd"GREEN("e"), result);
}
