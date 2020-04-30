#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
const int MAX_COUNT_OF_LINES = 10000;
const int MAX_SYMBOLS_IN_LINE = 1000;



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
    int *tableOfOffset;
    int numOfLine = 0;
    int symbolsRead;
    off_t sumOfOffset = 0;
    char *currLine;
    char *nameOfFile;
    int fileDescriptor;
    int line=1;
    tableOfOffset = calloc(MAX_COUNT_OF_LINES,sizeof(int));
    currLine = malloc(sizeof(char)*MAX_SYMBOLS_IN_LINE);
    nameOfFile = malloc(sizeof(char)*MAX_SYMBOLS_IN_LINE);
    fileDescriptor=open(argc[1],O_RDONLY);
    while(fileDescriptor==-1){
	printf("wrong name of file! try again:\n");
	scanf("%s",nameOfFile);
	fileDescriptor=open(nameOfFile,O_RDONLY);
    }
    symbolsRead = read(fileDescriptor,currLine,MAX_SYMBOLS_IN_LINE);  
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
	    lseek(fileDescriptor,sumOfOffset,SEEK_SET);
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
    close(fileDescriptor);
    free(currLine);
    free(tableOfOffset);
    free(nameOfFile);
    return 0;
}

