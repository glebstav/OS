#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
int main() {
	char *text = {"SoMe inTeResTing tExT\n"};

	FILE *receiver = popen("./recv.exe", "w");
	if(receiver == NULL) {
		perror("popen failed");
		return -1;
	}

	if(fprintf(receiver,"%s",text) != strlen(text) ){
		perror("can't print file");
		if(pclose(receiver) == -1){
			perror("can't pclose receiver");
		}
		return -1;	
	}

	if(pclose(receiver) == -1){
                perror("can't pclose receiver");
		return -1;
        }
	
	return 0;
}
