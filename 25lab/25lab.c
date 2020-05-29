#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFFER 256

int main(int argc, char **argv) {

	int fileDesc[2];
	pid_t recvPid, sendPid;
	if (pipe(fileDesc) == -1) {
		perror("pipe failed");
		return -1;
	}
	sendPid = fork();

	if(sendPid == 0){
		char *text = {"SoMe inTeResTing tExT\n"};
		if(write(fileDesc[0],text,strlen(text)) == -1){
			perror("can't write file");
			if(close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1){
				perror("can't close fileDesc");
			}
			return -1;	
		}

		if(close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1){
                        perror("can't close fileDesc");
			return -1;
                }
		
		return 0;
	}

	recvPid = fork();
	
	if(recvPid == 0){
		char text[BUFFER];
		int cntRead;

		while(1){
			cntRead = read(fileDesc[1],text,BUFFER);
			if(cntRead < 1){
				if(errno == EINTR){
					errno = 0;
					continue;
				} else 
					break;
			} else {
				for (int i = 0; i < cntRead; i++) {
					text[i] = toupper(text[i]);
				}
				
				if(write(1,text,cntRead) == -1){
					perror("can't write file");
					if (close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1){
			                        perror("close failed");
                       			}
					return -1;
				}
			}
		}

	        for(int i = 0;i < 2;i++){
                	if(wait(NULL) == -1){
                        	perror("wait failed");
                	}
        	}
		
		


		if (close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1) {
                        perror("close failed");
                        return -1;
                }
		if(cntRead == -1){
			perror("can't read file");
			return -1;
		}
		return 0;
	}




	if (sendPid == -1 || recvPid == -1 ) {
		if (close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1) {
			perror("close failed");
		}
		perror("fork failed");
		return -1;
	}

        if (close(fileDesc[0]) == -1 || close(fileDesc[1]) == -1) {
                perror("close failed");
		return -1;
        }

	return 0;
}
