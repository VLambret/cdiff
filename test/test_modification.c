#include "unity_src/unity.h"
#include <modification.h>
#include <string.h>
#include <stdbool.h>

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

// Helpers

#define NEXT ((void *) !NULL)

void TEST_ASSERT_EQUAL_MODIFICATION(struct modification *m, enum modification_type type, char *content, bool last) {
	int content_size = strlen(content);
	TEST_ASSERT_EQUAL_INT(type, m->type);
	TEST_ASSERT_EQUAL_INT(content_size, m->content_size);
	TEST_ASSERT_EQUAL_STRING_LEN(content, m->content, content_size);
	if (!last) {
		TEST_ASSERT_NOT_NULL(m->next);
	} else {
		TEST_ASSERT_NULL(m->next);
	}
}

// Extraction

void test_modification_extraction_with_two_empty_strings() {
	struct modification *m = extract_modification_steps("", "");
	TEST_ASSERT_NULL(m);
}

void test_modification_extraction_with_identical_text() {
	struct modification *m = extract_modification_steps("a", "a");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "a", true);
	modification_destroy(m);

	m = extract_modification_steps("abcd", "abcd");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "abcd", true);
	modification_destroy(m);
}

void test_modification_extraction_added_text_on_empty_string() {
	struct modification *m = extract_modification_steps("", "a");
	TEST_ASSERT_EQUAL_MODIFICATION(m, ADDING, "a", true);
	modification_destroy(m);

	m = extract_modification_steps("", "abcd");
	TEST_ASSERT_EQUAL_MODIFICATION(m, ADDING, "abcd", true);
	modification_destroy(m);
}

void test_modification_extraction_with_added_text_at_beginning() {
	struct modification *m = extract_modification_steps("b", "ab");
	TEST_ASSERT_EQUAL_MODIFICATION(m, ADDING, "a", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, TEXT, "b", true);
	modification_destroy(m);

	m = extract_modification_steps("bcde", "abcde");
	TEST_ASSERT_EQUAL_MODIFICATION(m, ADDING, "a", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, TEXT, "bcde", true);
	modification_destroy(m);
}

void test_modification_extraction_with_added_text_in_the_middle() {
	struct modification *m = extract_modification_steps("abde", "abcde");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "ab", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, ADDING, "c", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next->next, TEXT, "de", true);
	modification_destroy(m);
}

void test_modification_extraction_with_added_text_at_the_end() {
	struct modification *m = extract_modification_steps("abcd", "abcde");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "abcd", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, ADDING, "e", true);
	modification_destroy(m);
}

void test_modification_extraction_with_removing_single_char() {
	struct modification *m = extract_modification_steps("a", "");
	TEST_ASSERT_EQUAL_MODIFICATION(m, REMOVAL, "a", true);
	modification_destroy(m);
}

void test_modification_extraction_with_removing_from_the_beginning() {
	struct modification *m = extract_modification_steps("abcde", "bcde");
	TEST_ASSERT_EQUAL_MODIFICATION(m, REMOVAL, "a", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, TEXT, "bcde", true);
	modification_destroy(m);
}

void test_modification_extraction_with_removing_from_the_middle() {
	struct modification *m = extract_modification_steps("abcde", "abde");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "ab", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, REMOVAL, "c", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next->next, TEXT, "de", true);
	modification_destroy(m);
}

void test_modification_extraction_with_removing_from_the_end() {
	struct modification *m = extract_modification_steps("abcde", "abcd");
	TEST_ASSERT_EQUAL_MODIFICATION(m, TEXT, "abcd", false);
	TEST_ASSERT_EQUAL_MODIFICATION(m->next, REMOVAL, "e", true);
	modification_destroy(m);
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
