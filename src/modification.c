#include <string.h>
#include <stdio.h>
#include <modification.h>
#include <levenshtein.h>

struct modification *new_modification(enum modification_type type) {
	struct modification *m = malloc(sizeof(struct modification));
	if (m) {
		m->type = type;
		m->content = NULL;
		m->content_size = 0;
		m->next = NULL;
	}
	return m;
}

void modification_destroy(struct modification *m) {
	if (m) {
		modification_destroy(m->next);
		free(m);
	}
}

static enum modification_type get_shortest_distance_type(struct levenshtein_matrix *m, int y, int x) {
	int identical_char_distance = m->cost_matrix[(y - 1) * (m->width + 1) + (x - 1)];
	int added_char_distance = m->cost_matrix[y * (m->width + 1) + (x - 1)];

	if (identical_char_distance <= added_char_distance) {
		return TEXT;
	} else {
		return ADDING;
	}
}

struct modification *extract_modification_steps(const char *line1, const char *line2) {
	struct modification *current = NULL;
	struct levenshtein_matrix *m = new_levenshtein_matrix(line1, line2);

	int y = m->height;
	int x = m->width;

	while (y > 0 && x > 0) {
		enum modification_type type = get_shortest_distance_type(m, y, x);

		if (!current) {
			current = new_modification(type);
		} else if (current->type != type) {
			struct modification *previous = new_modification(type);
			current->content = &line2[x];
			previous->next = current;
			current = previous;
		}

		current->content_size++;
		if (type == TEXT) {
			if (y > 0) y--;
		}
		if (x > 0) x--;
	}

	if (current && current->type == TEXT) {
		current->content = line1;
	}
	if (x > 0) {
		struct modification *previous = new_modification(ADDING);
		previous->content = line2;
		previous->content_size = x;
		previous->next = current;
		current = previous;
	}

	destroy_levenshtein_matrix(m);	
	return current;
}

void modification_to_string(struct modification *m, char * result) {
	size_t index = 0;
	struct modification *current = m;
	while (current) {
		if (current->type == TEXT) {
			memcpy(&result[index], current->content, current->content_size);
			index = index + current->content_size;
		} else if (current->type == ADDING) {
			memcpy(&result[index],  "\x1b[32m", 5);
			memcpy(&result[index + 5], current->content, current->content_size);
			memcpy(&result[index + 5 + current->content_size],  "\x1b[0m", 4);
			index = index + 9 + current->content_size;
		}
		current = current->next;
	}
	result[index] = '\0';
}
