#include <stdio.h>

#define MAX_LINE_SIZE 4096

int print_file(FILE *file) {
	char line[MAX_LINE_SIZE];

	while(fgets(line, MAX_LINE_SIZE, file)) {
		printf("%s", line);
	}

	return 0;
}

int main(int argc, char **argv) {
	int rt = 1;
	if (argc != 3) {
		return 1;
	}

	FILE* file1 = fopen(argv[1], "r");
	if (!file1) {
		goto end;
	}

	FILE* file2 = fopen(argv[2], "r");
	if (!file2) {
		goto close_file1;
	}

	rt = print_file(file1);

	fclose(file2);
close_file1:
	fclose(file1);
end:
	return rt;
}
