#include <string.h>
#include <stdio.h>
#include <modification.h>
#include <levenshtein.h>

struct modification *new_modification(enum modification_type type, const char *content, struct modification *next) {
	struct modification *m = malloc(sizeof(struct modification));
	if (m) {
		m->type = type;
		m->content = content;
		m->content_size = 1;
		m->next = next;
	}
	return m;
}

void modification_destroy(struct modification *m) {
	if (m) {
		modification_destroy(m->next);
		free(m);
	}
}

void modification_increase(struct modification *m) {
	m->content--;
	m->content_size++;
}

static enum modification_type get_shortest_distance_type(struct levenshtein_matrix *m, int y, int x) {
	int current_distance = m->cost_matrix[y * (m->width + 1) + x];
	int identical_char_distance = m->cost_matrix[(y - 1) * (m->width + 1) + (x - 1)];
	int added_char_distance = m->cost_matrix[y * (m->width + 1) + (x - 1)];
	int removed_char_distance = m->cost_matrix[(y - 1) * (m->width + 1) + x];

	if (identical_char_distance <= added_char_distance && identical_char_distance <= removed_char_distance) {
		if (identical_char_distance == current_distance) {
			return TEXT;
		} else {
			// The previous char is replaced, we start start by ADDING to get a REMOVAL
			// ADDING modification
			return ADDING;
		}
	} else if (added_char_distance <= removed_char_distance) {
		return ADDING;
	} else {
		return REMOVAL;
	}
}

static struct modification *extract_modification_steps_non_trivial(const char *line1, const char *line2) {
	struct levenshtein_matrix *m = new_levenshtein_matrix(line1, line2);
	struct modification *head = NULL;

	int y = m->height;
	int x = m->width;

	while (y > 0 && x > 0) {
		enum modification_type type = get_shortest_distance_type(m, y, x);

		if (!head || head->type != type) {
			const char *content;
			if (type == REMOVAL) {
				content = &line1[y-1];
			} else {
				content = &line2[x-1];
			}
			struct modification *new_head = new_modification(type, content, head);
			head = new_head;
		} else {
			modification_increase(head);
		}

		if (type == TEXT || type == REMOVAL) {
			if (y > 0) y--;
		}
		if (type == TEXT || type == ADDING) {
			if (x > 0) x--;
		}
	}

	if (head && head->type == TEXT) {
		head->content = &line1[y];
	}
	if (x > 0) {
		struct modification *new_head = new_modification(ADDING, line2, head);
		new_head->content_size = x;
		head = new_head;
	}
	if (y > 0) {
		struct modification *new_head = new_modification(REMOVAL, line1, head);
		new_head->content_size = y;
		head = new_head;
	}

	destroy_levenshtein_matrix(m);
	return head;
}

struct modification *extract_modification_steps(const char *line1, const char *line2) {
	struct modification *current = NULL;

	if (line1[0] == '\0' && line2[0] == '\0') {
		return NULL;
	}

	if (line1[0] == '\0') {
		current = new_modification(ADDING, line2, NULL);
		current->content_size = strlen(line2);
		return current;
	}

	if (line2[0] == '\0') {
		current = new_modification(REMOVAL, line1, NULL);
		current->content_size = strlen(line1);
		return current;
	}

	return extract_modification_steps_non_trivial(line1, line2);
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
		} else if (current->type == REMOVAL) {
			memcpy(&result[index],  "\x1b[31m", 5);
			memcpy(&result[index + 5], current->content, current->content_size);
			memcpy(&result[index + 5 + current->content_size],  "\x1b[0m", 4);
			index = index + 9 + current->content_size;
		}
		current = current->next;
	}
	result[index] = '\0';
}
