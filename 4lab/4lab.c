#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define BUFFER 256
int main() {
	int cntOfAlloc = 2;
	int cntOfLines = 0;
	char** node;
	node = malloc(sizeof(char*)*cntOfAlloc);
	if (node == NULL) {
		perror("can't allocate");
		return 1;
	}
	char line[BUFFER];
	while (1) {

			//reading
		
		if (fgets(line, BUFFER, stdin) == NULL) {
			perror("reading error\n");
			return 1;
		}

		fflush(stdin);

		if (line[0] == '.' && strlen(line) == 2) {
			break;
		}

			//writing
	
		node[cntOfLines] = calloc(BUFFER,sizeof(char));
		strncpy(node[cntOfLines],line,BUFFER);
		cntOfLines++;		

			//realloc

		if(cntOfAlloc == cntOfLines + 1){
			node = realloc(node,cntOfAlloc*2*sizeof(char*));
			if(node == NULL){
				perror("realloc failed");
				return 1;
			}
			cntOfAlloc*=2;
		}
	}
	for(int i = 0;i<cntOfLines;i++){
		printf("%s",node[i]);
		free(node[i]);
	}
	free(node);
	return 0;
}
