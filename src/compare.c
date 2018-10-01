#include <compare.h>
#include <modification.h>

void line_compare(const char *line1, const char *line2, char *result) {
	struct modification m = extract_modification_steps(line1, line2);
	modification_to_string(&m, result);
	return;
}
