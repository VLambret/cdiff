#include <string.h>
#include <compare.h>
#include <stdio.h>

void line_compare(const char *line1, const char *line2, char *result) {
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
