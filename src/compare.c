#include <string.h>
#include <compare.h>

void line_compare(const char *line1, const char *line2, char *result) {
	while (*line1 || *line2) {
		if (*line2 == *line1) {
			*result = *line1;
			line2++;
			line1++;
			result++;
		} else {
			*result++ = '[';
			*result++ = *line2;
			*result++ = ']';
			*line2++;
		}
	}
}
