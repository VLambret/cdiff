#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "levenshtein.h"

uint32_t min3(uint32_t val1, uint32_t val2, uint32_t val3) {
	if (val1 <= val2 && val1 <= val3)
		return val1;
	if (val2 <= val3)
		return val2;
	return val3;
}

bool is_identity(struct levenshtein_matrix *m, int y, int x) {
	return m->str1[y] == m->str2[x];
}

static uint32_t matrix_cost(struct levenshtein_matrix *m, int y, int x) {
	return m->cost_matrix[y * (m->width + 1) + x];
}

static void matrix_set_cost(struct levenshtein_matrix *m, int y, int x, uint32_t cost) {
	m->cost_matrix[y * (m->width + 1) + x] = cost;
}

static void initialize_matrix(struct levenshtein_matrix *m) {
	for (int y = 0; y <= m->height; y++) {
		matrix_set_cost(m, y, 0, y);
	}
	for (int x = 0; x <= m->width; x++) {
		matrix_set_cost(m, 0, x, x);
	}
}

static uint32_t compute_lowest_cost(struct levenshtein_matrix *m, int y, int x) {
	uint32_t previous_cost = matrix_cost(m, y - 1, x - 1);
	if (is_identity(m, y - 1, x - 1)) {
		return previous_cost;
	}

	uint32_t remove_cost = matrix_cost(m, y, x - 1);
	uint32_t insert_cost = matrix_cost(m, y - 1, x);
	uint32_t substitute_cost = previous_cost;

	return min3(remove_cost, insert_cost, substitute_cost) + 1;
}

static void fill_matrix(struct levenshtein_matrix *m) {
	for (int y = 1; y <= m->height; y++) {
		for (int x = 1; x <= m->width; x++) {
			matrix_set_cost(m, y, x, compute_lowest_cost(m, y, x));
		}
	}
}

struct levenshtein_matrix *new_levenshtein_matrix(const char *string1, const char *string2) {
	struct levenshtein_matrix *m = malloc(sizeof(struct levenshtein_matrix));
	m->height = strlen(string1);
	m->width = strlen(string2);
	m->str1 = strdup(string1);
	m->str2 = strdup(string2);
	m->cost_matrix = malloc((m->height + 1) * (m->width + 1) * sizeof(uint32_t));

	if (!m->str1 || !m->str2 || !m->cost_matrix) {
		return NULL;
	}

	initialize_matrix(m);
	fill_matrix(m);

	return m;
}

void destroy_levenshtein_matrix(struct levenshtein_matrix *m) {
	free(m->str1);
	free(m->str2);
	free(m->cost_matrix);
	free(m);
}
