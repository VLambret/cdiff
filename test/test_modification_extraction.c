#include "unity_src/unity.h"
#include <modification.h>
#include <string.h>
#include <stdbool.h>

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
