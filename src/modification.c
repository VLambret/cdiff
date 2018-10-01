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
	struct modification result = {TEXT, line1, 0, NULL};
	struct modification *current = &result;

	while (*line1 || *line2) {
		if (*line2 == *line1) {
			if (!current || current->type != TEXT) {
				struct modification *tmp = new_modification(TEXT);
				current->next = tmp;
				current = tmp;
				current->content = line1;
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
			}
			current->content_size++;
			line2++;
		}
	}
	return result;
}

void modification_to_string(struct modification *m, char * result_init) {
	char *result = result_init;
	struct modification *current = m;
	while (current) {
		if (current->type == TEXT) {
			result = result + snprintf(result, current->content_size + 1, "%s", current->content);
		} else {
			result = result + sprintf(result, "\x1b[32m");
			result = result + snprintf(result, current->content_size + 1, "%s", current->content);
			result = result + sprintf(result, "\x1b[0m");
		}
		current = current->next;
	}
}
