#pragma once

#include <stdint.h>

struct levenshtein_matrix {
	char *str1;
	char *str2;
	size_t width;
	size_t height;
	uint32_t *cost_matrix;
};

struct levenshtein_matrix *new_levenshtein_matrix(const char *string1, const char *string2);

void destroy_levenshtein_matrix(struct levenshtein_matrix *m);

