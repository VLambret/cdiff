#include <stdlib.h>
#include <string.h>
#include <compare.h>
#include <stdio.h>

enum modification_type {TEXT, ADDING};

struct modification {
	enum modification_type type;
	const char * content;
	size_t content_size;
	struct modification *next;
};

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
			line2++;
		}
	}
	return result;
}

void modification_to_string(struct modification *m, char * result) {
	struct modification *current = m;
	while (current) {
		result = result + snprintf(result, m->content_size + 1, "%s", m->content);
		current = current->next;
	}
	*result = '\0';
}

void line_compare(const char *line1, const char *line2, char *result) {
	struct modification m = extract_modification_steps(line1, line2);
	modification_to_string(&m, result);
	return;

	while (*line1 || *line2) {
		if (*line2 == *line1) {
			*result = *line1;
			line2++;
			line1++;
			result++;
		} else {
			result = result + sprintf(result, "\x1b[32m%c\x1b[0m", *line2);
			line2++;
		}
	}
}
