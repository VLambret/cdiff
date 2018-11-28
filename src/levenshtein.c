#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "levenshtein.h"

static int cost(struct levenshtein_matrix *m, int y, int x) {
	if (m->str1[y] == m->str2[x]) {
		return 0;
	} else {
		return 1;
	}
}

static void initialize_matrix(struct levenshtein_matrix *m) {
	for (int y = 0; y <= m->height; y++) {
		m->cost_matrix[y * (m->width + 1) + 0] = y;
	}
	for (int x = 0; x <= m->width; x++) {
		m->cost_matrix[0 * (m->width + 1) + x] = x;
	}
}

static uint32_t matrix_cost(struct levenshtein_matrix *m, int y, int x) {
	return m->cost_matrix[y * (m->width + 1) + x];
}

static uint32_t compute_lowest_cost(struct levenshtein_matrix *m, int y, int x) {
	uint32_t remove_cost = matrix_cost(m, y, x - 1) + 1;
	uint32_t insert_cost = matrix_cost(m, y - 1, x) + 1;
	uint32_t substitute_cost = matrix_cost(m, y - 1, x - 1) + cost(m, y - 1, x - 1);

	if (substitute_cost <= remove_cost && substitute_cost <= insert_cost) {
		return substitute_cost;
	} else if (remove_cost <= insert_cost) {
		return remove_cost;
	} else {
		return insert_cost;
	}
}

static void fill_matrix(struct levenshtein_matrix *m) {
	for (int y = 1; y <= m->height; y++) {
		for (int x = 1; x <= m->width; x++) {
			m->cost_matrix[y * (m->width + 1) + x] = compute_lowest_cost(m, y, x);
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
