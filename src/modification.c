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

void modification_destroy(struct modification *m) {
	if (m) {
		modification_destroy(m->next);
		free(m);
	}
}

struct modification *extract_modification_steps(const char *line1, const char *line2) {
	struct modification *head = NULL;
	struct modification *current = NULL;
	enum modification_type previous_type = UNDEFINED;

	while (*line1 || *line2) {
		enum modification_type current_type;
		if (*line2 == *line1) {
			current_type = TEXT;
		} else {
			current_type = ADDING;
		}
		if (!current) {
			head = current = new_modification(current_type);
			current->content = line2;
		} else if (current_type != previous_type) {
			current->next = new_modification(current_type);
			current = current->next;
			current->content = line2;
		}
		current->content_size++;
		line2++;
		if (current_type == TEXT) {
			line1++;
		}
		previous_type = current_type;
		continue;
	}

	if (!head) {
		head = new_modification(UNDEFINED);
	}
	return head;
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
