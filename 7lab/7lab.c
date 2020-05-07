#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
const int MAX_COUNT_OF_LINES = 10000;
const int MAX_SYMBOLS_IN_LINE = 1000;
const int TIMEOUT = 5000;

void freeAll(unsigned int cnt, ...){
    void* pointer = &cnt + 1;
    while(cnt--){
	free(pointer);
	pointer++;	
    }

}


int SearchNextLine(char *line,int *pos,int symbolsInLine){
   int lenght = 0;
   while(*pos<symbolsInLine && line[*pos]!='\n'){
	(*pos)++;
	lenght++;
	}
   if(line[*pos]=='\n'){
	(*pos)++;
	lenght++;
   }
   if(*pos == symbolsInLine){
        *pos=-1;
	}
 
   return lenght;
}


void LineDivider(char *currLine,int *numOfLine, int *tableOfOffset, int symbolsInLine){

 int currPos = 0,lenghtOfLine;
	while(currPos!=-1){
            lenghtOfLine = SearchNextLine(currLine,&currPos,symbolsInLine);
	    tableOfOffset[*numOfLine] += lenghtOfLine;

            if(currPos!=-1){
                (*numOfLine)++;
            }
	           

        }

}


int main(int argv,char **argc) {
    unsigned int needToFree = 0;  
    int *tableOfOffset;
    int numOfLine = 0;
    char* file;
    off_t sumOfOffset = 0;
    char *nameOfFile;
    int lenghtOfFile;
    int fileDescriptor;
    int line=1;
    struct pollfd consolePoll[1];
    int timeleft = 1;
    
	//memory allocation

    consolePoll[0].fd = 0;
    consolePoll[0].events = POLLIN;

    tableOfOffset = calloc(MAX_COUNT_OF_LINES,sizeof(int));
    if(tableOfOffset==0){
	perror("malloc failed");
	return 0;
    }
    needToFree++;
    nameOfFile = malloc(sizeof(char)*MAX_SYMBOLS_IN_LINE);
    if(nameOfFile==0){
	perror("malloc failed");
	freeAll(needToFree,tableOfOffset);
	return 0;
    }
    needToFree++;

	//file opening

    fileDescriptor=open(argc[1],O_RDONLY);
    while(fileDescriptor==-1){
	printf("wrong name of file! try again:\n");
	scanf("%s",nameOfFile);
	fileDescriptor=open(nameOfFile,O_RDONLY);
    }

	//file mapping

    lenghtOfFile = lseek(fileDescriptor,0,SEEK_END);
    if(lenghtOfFile==-1){
        perror("lseek failed");
        freeAll(needToFree,tableOfOffset,nameOfFile);
        if(close(fileDescriptor)==-1){
            perror("can't close file");
        }
        return -1;
    }
    
    file = mmap(0,lenghtOfFile,PROT_READ,MAP_PRIVATE,fileDescriptor,0);
    if(file==-1){
	perror("can't map file");
	freeAll(needToFree,tableOfOffset,nameOfFile);
	if(close(fileDescriptor)==-1){
	    perror("can't close file");
	    return -1;
	}
	return 0;
    }	  

	//file proccessing

    	
	LineDivider(file,&numOfLine,tableOfOffset,lenghtOfFile);
       

	//end of text proccessing

    printf("write number of line\n");    
    timeleft = poll(consolePoll,1,TIMEOUT);
    
    if(timeleft == -1){
	perror("poll fail");
	freeAll(needToFree,tableOfOffset,nameOfFile);
        munmap(file,lenghtOfFile);
	if(close(fileDescriptor)==-1){
            perror("can't close file");
            return -1;
        }
    return 0;
    }

    while(timeleft>0){
	if(consolePoll[0].revents == POLLIN){
	    while(line!=0){
		timeleft = -1;
		if(scanf("%d",&line)==1 && line>0 && line<=numOfLine+1){
	            sumOfOffset=0;	
		    for(int i=0;i<line-1;i++){
		        sumOfOffset+=tableOfOffset[i];
		    }
		    write(2,file+sumOfOffset,tableOfOffset[line-1]);		    
		} else {
	 	    if(line!=0){
		        printf("wrong number of line!\n");
		        fflush(stdin);
		    }
		}
	    }
    	}
    }

	//TIMEOUT

    if(timeleft==0){
	write(2,file,lenghtOfFile);
    } 

    
    freeAll(needToFree,tableOfOffset,nameOfFile);
    munmap(file,lenghtOfFile);
    if(close(fileDescriptor)==-1){
        perror("can't close file");
        return -1;
    }

    return 0;
}

