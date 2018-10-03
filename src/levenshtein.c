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

static void print_levenshtein_matrix(struct levenshtein_matrix *m) {
	return;
	printf(" | ");
	for (int x = 0; x < m->width; x++) {
		printf("%c", m->str2[x]);
	}
	printf("\n-+");
	for (int x = 0; x <= m->width; x++) {
		printf("-");
	}
	printf("\n");

	for (int y = 0; y <= m->height; y++) {
		if (y == 0) {
			printf(" |");
		} else {
			printf("%c|", m->str1[y - 1]);
		}
		for (int x = 0; x <= m->width; x++) {
			printf("%d", cost(m, y, x));
		}
		printf("\n");
	}

	printf("\n");
	for (int y = 0; y <= m->height; y++) {
		for (int x = 0; x <= m->width; x++) {
			printf("%d ", m->cost_matrix[y * (m->width +1) + x]);
		}
		printf("\n");
	}
}

struct levenshtein_matrix *new_levenshtein_matrix(const char *string1, const char *string2) {
	struct levenshtein_matrix *m = malloc(sizeof(struct levenshtein_matrix));
	m->height = strlen(string1);
	m->width = strlen(string2);
	m->str1 = strdup(string1);
	m->str2 = strdup(string2);
	m->cost_matrix = malloc((m->height + 1) * (m->width + 1) * sizeof(int));

	if (!m->str1 || !m->str2 || !m->cost_matrix) {
		return NULL;
	}

	for (int y = 0; y <= m->height; y++) {
		m->cost_matrix[y * (m->width + 1) + 0] = y;
	}
	for (int x = 0; x <= m->width; x++) {
		m->cost_matrix[0 * (m->width + 1) + x] = x;
	}

	for (int y = 1; y <= m->height; y++) {
		for (int x = 1; x <= m->width; x++) {
			char remove_cost = m->cost_matrix[y * (m->width + 1) + (x - 1)] + 1;
			char insert_cost = m->cost_matrix[(y - 1) * (m->width + 1) + x] + 1;
			char substitute_cost = m->cost_matrix[(y - 1) * (m->width + 1) + (x - 1)] + cost(m, y - 1, x - 1);

			if (substitute_cost <= remove_cost && substitute_cost <= insert_cost) {
				m->cost_matrix[y * (m->width + 1) + x] = substitute_cost;
			} else if (remove_cost <= insert_cost) {
				m->cost_matrix[y * (m->width + 1) + x] = remove_cost;
			} else {
				m->cost_matrix[y * (m->width + 1) + x] = insert_cost;
			}
		}
	}
	print_levenshtein_matrix(m);

	return m;
}

void destroy_levenshtein_matrix(struct levenshtein_matrix *m) {
	free(m->str1);
	free(m->str2);
	free(m->cost_matrix);
	free(m);
}
