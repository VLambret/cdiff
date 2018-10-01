#pragma once

#include <stdlib.h>

enum modification_type {UNDEFINED, TEXT, ADDING};

struct modification {
	enum modification_type type;
	const char * content;
	size_t content_size;
	struct modification *next;
};

struct modification extract_modification_steps(const char *line1, const char *line2);

void modification_to_string(struct modification *m, char * result_init);
