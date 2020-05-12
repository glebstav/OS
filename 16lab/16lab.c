#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

int main(){
	int fd;
	char symbol;
	struct termios* termios_p;
	struct termios previous_conf;
	int reading;
	termios_p = malloc(sizeof(struct termios));

		//terminal opening 

	fd = open("/dev/tty",O_RDONLY);
	if(isatty(fd) != 1){
		if(errno == ENOTTY){
			perror("this is not a terminal");
			if(close(fd) == -1)
				perror("can't close file");
		} else
			perror("can't open file");
		free(termios_p);
		return 1;	
	}
		
		//set config

	tcgetattr(fd,termios_p);
	previous_conf = *termios_p;
	termios_p->c_lflag &= ~(ICANON);
	termios_p->c_cc[VMIN]=1;
	termios_p->c_cc[VTIME]=0;
	tcsetattr(fd,TCSANOW,termios_p);

		//main part

	printf("Do you wanna eat?(y\\n)\n");
	while(reading = read(fd,&symbol,sizeof(char)) >= 0 && symbol !='y' && symbol != 'n'){
		printf("\nType 'y' or 'n'\n");
	}

	tcsetattr(fd,TCSANOW,&previous_conf);
	if(reading==-1){
		perror("cant read from terminal");
		free(termios_p);
		if(close(fd) == -1){
                perror("can't close file");
                return 1;
        }

	

	}
	if(symbol == 'y')
		printf("\nWell, go ahead and eat\n");
	else
		printf("\nSo, everything in its right place\n");

	free(termios_p);
	if(close(fd) == -1){
                perror("can't close file");
		return -1;
	}
	
	return 0;
}
