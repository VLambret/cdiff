#include "unity_src/unity.h"
#include <modification.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

// Extraction

void test_modification_extraction_with_identical_text() {
	struct modification m = extract_modification_steps("abcd", "abcd");
	TEST_ASSERT_EQUAL_INT(TEXT, m.type);
	TEST_ASSERT_EQUAL_INT(4, m.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("abcd", m.content, 4);
	TEST_ASSERT_NULL(m.next);
}

void test_modification_extraction_with_only_added_text() {
	struct modification m = extract_modification_steps("", "abcd");
	TEST_ASSERT_EQUAL_INT(ADDING, m.type);
	TEST_ASSERT_EQUAL_INT(4, m.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("abcd", m.content, 4);
	TEST_ASSERT_NULL(m.next);
}

void test_modification_extraction_with_added_text_at_begining() {
	struct modification m = extract_modification_steps("bcde", "abcde");
	struct modification suffix;

	TEST_ASSERT_EQUAL_INT(ADDING, m.type);
	TEST_ASSERT_EQUAL_INT(1, m.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("a", m.content, 1);
	TEST_ASSERT_NOT_NULL(m.next);

	suffix = *m.next;

	TEST_ASSERT_EQUAL_INT(TEXT, suffix.type);
	TEST_ASSERT_EQUAL_INT(4, suffix.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("bcde", suffix.content, 4);
	TEST_ASSERT_NULL(suffix.next);
}

void test_modification_extraction_with_added_text_in_the_middle() {
	struct modification m = extract_modification_steps("abcd", "abcde");
	struct modification suffix;
	struct modification prefix;

	TEST_ASSERT_EQUAL_INT(TEXT, m.type);
	TEST_ASSERT_EQUAL_INT(4, m.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("abcd", m.content, 4);
	TEST_ASSERT_NOT_NULL(m.next);

	suffix = *m.next;

	TEST_ASSERT_EQUAL_INT(ADDING, suffix.type);
	TEST_ASSERT_EQUAL_INT(1, suffix.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("e", suffix.content, 1);
	TEST_ASSERT_NULL(suffix.next);
}

void test_modification_extraction_with_added_text_at_the_end() {
	struct modification m = extract_modification_steps("abde", "abcde");
	struct modification added;
	struct modification suffix;

	TEST_ASSERT_EQUAL_INT(TEXT, m.type);
	TEST_ASSERT_EQUAL_INT(2, m.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("ab", m.content, 2);
	TEST_ASSERT_NOT_NULL(m.next);

	added = *m.next;

	TEST_ASSERT_EQUAL_INT(ADDING, added.type);
	TEST_ASSERT_EQUAL_INT(1, added.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("c", added.content, 1);
	TEST_ASSERT_NOT_NULL(added.next);

	suffix = *added.next;

	TEST_ASSERT_EQUAL_INT(TEXT, suffix.type);
	TEST_ASSERT_EQUAL_INT(2, suffix.content_size);
	TEST_ASSERT_EQUAL_STRING_LEN("de", suffix.content, 2);
	TEST_ASSERT_NULL(suffix.next);
}


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

void test_modification_string_modification_with_added_text_at_the_begining() {
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
