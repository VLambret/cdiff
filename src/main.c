#include <stdio.h>
#include "compare.h"

int main(int argc, char **argv) {
	char result[4096];
	if (argc != 3) {
		return 1;
	}

	line_compare(argv[1], argv[2], result);
	printf("%s\n", result);

	return 0;
}
