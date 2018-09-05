#include <stdio.h>

#define BUF_SIZE 4096

int print_first_file(FILE *file) {
	char buffer[BUF_SIZE];

	while(!feof(file)) {
		size_t read_nb;
		size_t write_nb = 0;
		read_nb = fread(buffer, sizeof(char), BUF_SIZE, file);
		while (write_nb < read_nb) {
			write_nb = write_nb + fwrite(&buffer[write_nb], sizeof(char), read_nb - write_nb, stdout);
		}
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

	rt = print_first_file(file1);

	fclose(file2);
close_file1:
	fclose(file1);
end:
	return rt;
}
