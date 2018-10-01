#include <string.h>
#include <stdio.h>
#include <modification.h>

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

struct modification extract_modification_steps(const char *line1, const char *line2) {
	struct modification *result = new_modification(UNDEFINED);
	struct modification *current = result;

	while (*line1 || *line2) {
		if (*line2 == *line1) {
			if (!current || current->type != TEXT) {
				struct modification *tmp = new_modification(TEXT);
				current->next = tmp;
				current = tmp;
				current->content = line1;
				if (result->type == UNDEFINED) {
					result = current;
				}
			}
			current->content_size++;
			line2++;
			line1++;
		} else {
			if (!current || current->type != ADDING) {
				struct modification *tmp = new_modification(ADDING);
				current->next = tmp;
				current = tmp;
				current->content = line2;
				if (result->type == UNDEFINED) {
					result = current;
				}
			}
			current->content_size++;
			line2++;
		}
	}

	if (result->type == UNDEFINED) {
		result->type = TEXT;
	}
	return *result;
}

void modification_to_string(struct modification *m, char * result) {
	size_t index = 0;
	struct modification *current = m;
	while (current) {
		if (current->type == TEXT) {
			memcpy(&result[index], current->content, current->content_size);
			index = index + current->content_size;
		} else {
			memcpy(&result[index],  "\x1b[32m", 5);
			memcpy(&result[index + 5], current->content, current->content_size);
			memcpy(&result[index + 5 + current->content_size],  "\x1b[0m", 4);
			index = index + 9 + current->content_size;
		}
		current = current->next;
	}
	result[index] = '\0';
}
