#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
const int MAX_COUNT_OF_LINES = 10000;
const int MAX_SYMBOLS_IN_LINE = 1000;

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
    int symbolsRead;
    off_t sumOfOffset = 0;
    char *currLine;
    char *nameOfFile;
    int fileDescriptor;
    int line=1;

	//memory allocation

    tableOfOffset = calloc(MAX_COUNT_OF_LINES,sizeof(int));
    if(tableOfOffset==0){
	perror("malloc failed");
	return 0;
    }
    needToFree++;
    currLine = malloc(sizeof(char)*MAX_SYMBOLS_IN_LINE);
    if(currLine == 0){
	perror("malloc failed");
	freeAll(needToFree,tableOfOffset);
	return 0;
    }
    needToFree++;
    nameOfFile = malloc(sizeof(char)*MAX_SYMBOLS_IN_LINE);
    if(nameOfFile==0){
	perror("malloc failed");
	freeAll(needToFree,tableOfOffset,currLine);
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

	//file reading

    symbolsRead = read(fileDescriptor,currLine,MAX_SYMBOLS_IN_LINE);
    while(symbolsRead<0){
	if(errno == EINTR){
	    errno = 0;
	    symbolsRead = read(fileDescriptor,currLine,MAX_SYMBOLS_IN_LINE);
	    continue;
	}
	perror("can't read from file");
	freeAll(needToFree,tableOfOffset,currLine,nameOfFile);
	if(close(fileDescriptor)==-1){
	    perror("can't close file");
	    return -1;
	}
	return 0;
    }	  

	//file proccessing

    while(symbolsRead==MAX_SYMBOLS_IN_LINE){
	
	LineDivider(currLine,&numOfLine,tableOfOffset,symbolsRead);
	symbolsRead = read(fileDescriptor,currLine,MAX_SYMBOLS_IN_LINE);
    }
    
    LineDivider(currLine,&numOfLine,tableOfOffset,symbolsRead);
    

	//end of text proccessing

    printf("write number of line\n");

    while(line!=0){
	if(scanf("%d",&line)==1 && line>0 && line<=numOfLine+1){
            sumOfOffset=0;	
	    for(int i=0;i<line-1;i++){
	        sumOfOffset+=tableOfOffset[i];
	    }

	    if(lseek(fileDescriptor,sumOfOffset,SEEK_SET)==-1){
		perror("lseek failed");
		freeAll(needToFree,currLine,tableOfOffset,nameOfFile);
		if(close(fileDescriptor)==-1){
	            perror("can't close file");
        	}
	        return -1;
	    }
	

	    read(fileDescriptor,currLine,tableOfOffset[line-1]);
	    for(int i=0;i<tableOfOffset[line-1];i++){
	        printf("%c",currLine[i]);
	    }
	    
	} else {
 	    if(line!=0){
	        printf("wrong number of line!\n");
	        fflush(stdin);
	    }
	}
    }
    freeAll(needToFree,currLine,tableOfOffset,nameOfFile);
    if(close(fileDescriptor)==-1){
            perror("can't close file");
            return -1;
    }

    return 0;
}

