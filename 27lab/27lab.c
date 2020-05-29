#include <stdio.h>
#define BUFFER 256

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return 0;
	}

	FILE *file, *prog;
	char text[BUFFER];
	if ((file = fopen(argv[1], "r")) == NULL) {
		perror("fopen failed");
		return -1;
	}
	prog = popen("wc -l", "w");
	if (prog == NULL) {
		perror("popen failed");
		return -1;
	}


	while (fgets(text, BUFFER, file) != NULL) {
		if (text[0] == '\n') {
			fputs(text, prog);
		}
	}


	if (fclose(file) == EOF) {
		perror("fclose failed");
		return -1;
	}
	if (pclose(prog) == -1) {
		perror("pclose failed");
		return -1;
	}

	return 0;
}
