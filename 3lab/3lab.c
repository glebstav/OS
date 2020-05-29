#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

    	if (argc != 2) {
        	printf("usage: %s file\n",argv[0]);
        	return 0;
	}
	
	printf("Real user id %ld\n", getuid());
        printf("Effective user id %ld\n", geteuid());


	FILE *file;
	file = fopen(argv[1], "r");
	if (file == NULL) {
        	perror("can't open file");
		return 1;
	}

	if(fclose(file) == -1){
		perror("can't close file");
		return 1;
	}
	
	uid_t userID = getuid();
	seteuid(userID);

 	printf("Set user ID \n");
	printf("Real user id %ld\n", getuid());
	printf("Effective user id %ld\n", geteuid());

	file = fopen(argv[1], "r");
        if (file == NULL) {
                perror("can't open file");
                return 1;
        }

        if(fclose(file) == -1){ 
                perror("can't close file");
                return 1;
        }

	return 0;
}


