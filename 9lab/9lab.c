 #include <sys/types.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <wait.h>
 #include <stdio.h>
 int command(char **);

 /* run a shell command from C program */

 int command(char **filename){
	pid_t chpid, waiting;
 	int status;

	if ((chpid = fork()) == 0) {
		execvp("cat", filename);
 		exit(127);
	 }

 	while ((waiting = wait(&status)) != chpid && waiting != -1)
		 ; /* null */
 	if (waiting== -1)
 		return(-1);
	else
 		return(WEXITSTATUS(status));
 }

 #if DEBUG
main(int argc, char **argv){
	if(argc!=2){
		printf("usage : %s \"filename\"",argv[0]);
	}
 	int result = command(argv);
	if(result!=-1){
		printf("***cat completed correctly***\n");
	} else {
		printf("***can't execute cat***\n");
		perror("can't create a child process");
	}
}
 #endif
