#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#define BUFFER 256

int main(int argc, char **argv) {
	char text[BUFFER];
	int cntRead;

	while(1){
		cntRead = read(0,text,BUFFER);
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
				return -1;
			}
		}
	}
	if (cntRead == -1) {
		perror("read failed");
		return -1;
	}
	return 0;
}
