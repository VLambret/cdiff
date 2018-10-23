#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compare.h"

#define COLOR_OVERHEAD_SIZE 10

static void print_usage(FILE *output, char *executable) {
	fprintf(output, "usage : %s string1 string2\n", executable);
}

int main(int argc, char **argv) {
	char *result;

	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_usage(stdout, argv[0]);
		exit(EXIT_SUCCESS);

	}

	if (argc != 3) {
		fprintf(stderr, "error : type %s --help for help\n", argv[0]);
		print_usage(stderr, argv[0]);
		exit(EXIT_FAILURE);
	}

	result = malloc((strlen(argv[1]) + strlen(argv[2])) * COLOR_OVERHEAD_SIZE);
	if (!result) {
		perror("Cannot allocate memory for result");
		exit(EXIT_FAILURE);
	}

	line_compare(argv[1], argv[2], result);
	printf("%s\n", result);
	free(result);

	return EXIT_SUCCESS;
}
