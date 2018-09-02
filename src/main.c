#include <stdio.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		return 1;
	}

	FILE* file1 = fopen(argv[1], "r");
	if (!file1) {
		return 1;
	}

	fclose(file1);
	return 0;
}
