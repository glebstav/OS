 #include <sys/types.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <wait.h>
 #include <stdio.h>
 int command(char **);

 /* run a shell command from C program */

 int command(char **arguments){
	pid_t chpid, waiting;
 	int status;
	char *programm = arguments[1];
	char **arg_p = arguments+1;
	if ((chpid = fork()) == 0) {
		execvp(programm,arg_p);
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

	if(argc<2){
		printf("usage : %s \"programm\" \"argumets ...\"\n",argv[0]);
	} else {
		printf("%d\n",command(argv));
	}

}
#endif
