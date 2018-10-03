#include "unity_src/unity.h"
#include <levenshtein.h>

// Extraction

void test_levenshtein_matrix_of_similar_strings() {
	struct levenshtein_matrix *m = new_levenshtein_matrix("abc", "abc");
	int expected_cost_matrix[] = {0, 1, 2, 3,
	                              1, 0, 1, 2,
	                              2, 1, 0, 1,
	                              3, 2, 1, 0};
	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL_INT(3, m->height);
	TEST_ASSERT_EQUAL_INT(3, m->width);
	TEST_ASSERT_EQUAL_STRING("abc", m->str1);
	TEST_ASSERT_EQUAL_STRING("abc", m->str2);
	TEST_ASSERT_EQUAL_INT_ARRAY(expected_cost_matrix, m->cost_matrix, 16);
	destroy_levenshtein_matrix(m);
}

void test_levenshtein_matrix_of_similar_strings_other_case() {
	struct levenshtein_matrix *m = new_levenshtein_matrix("1234", "1234");
	int expected_cost_matrix[] = {0, 1, 2, 3, 4,
	                              1, 0, 1, 2, 3,
	                              2, 1, 0, 1, 2,
	                              3, 2, 1, 0, 1,
	                              4, 3, 2, 1, 0};
	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL_INT(4, m->height);
	TEST_ASSERT_EQUAL_INT(4, m->width);
	TEST_ASSERT_EQUAL_STRING("1234", m->str1);
	TEST_ASSERT_EQUAL_STRING("1234", m->str2);
	TEST_ASSERT_EQUAL_INT_ARRAY(expected_cost_matrix, m->cost_matrix, 25);
	destroy_levenshtein_matrix(m);
}

void test_levenshtein_matrix_with_single_added_char() {
	struct levenshtein_matrix *m = new_levenshtein_matrix("abc", "abcd");
	int expected_cost_matrix[] = {0, 1, 2, 3, 4,
	                              1, 0, 1, 2, 3,
	                              2, 1, 0, 1, 2,
	                              3, 2, 1, 0, 1};
	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL_INT(3, m->height);
	TEST_ASSERT_EQUAL_INT(4, m->width);
	TEST_ASSERT_EQUAL_STRING("abc", m->str1);
	TEST_ASSERT_EQUAL_STRING("abcd", m->str2);
	TEST_ASSERT_EQUAL_INT_ARRAY(expected_cost_matrix, m->cost_matrix, 20);
	destroy_levenshtein_matrix(m);
}

void test_levenshtein_matrix_with_single_removed_char() {
	struct levenshtein_matrix *m = new_levenshtein_matrix("abcd", "abc");
	int expected_cost_matrix[] = {0, 1, 2, 3,
	                              1, 0, 1, 2,
	                              2, 1, 0, 1,
	                              3, 2, 1, 0,
	                              4, 3, 2, 1};
	TEST_ASSERT_NOT_NULL(m);
	TEST_ASSERT_EQUAL_INT(4, m->height);
	TEST_ASSERT_EQUAL_INT(3, m->width);
	TEST_ASSERT_EQUAL_STRING("abcd", m->str1);
	TEST_ASSERT_EQUAL_STRING("abc", m->str2);
	TEST_ASSERT_EQUAL_INT_ARRAY(expected_cost_matrix, m->cost_matrix, 20);
	destroy_levenshtein_matrix(m);
}
