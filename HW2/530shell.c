//C program John Woltz
// Comp 530
// On my honor, I pledge that I have neither given nor received unauthorized aid on this assignment
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
#include <unistd.h>
#define MAX_INPUT 80
 main(){
 char fileName[MAX_INPUT];
 int childPID;
 char *holder;
 char *args[MAX_INPUT];
 char input[MAX_INPUT];
 
 int fileCounter = 0;
 int arraySequence =0;
 int charSequence=0;
 int holderSequence=0;
int i = 0;

 
while(1==1){
	printf("%c ",'%');
	fgets(input, MAX_INPUT, stdin);
	i=0;
	
	while(i<MAX_INPUT){
		if(input[i]==EOF){
			exit(0);
		}
		i++;
	}

	args[i] = strtok(input," ");

	while(args[i]!=NULL)
	{
		args[++i] = strtok(NULL," ");
	}



	i=0;
	childPID=fork();

	if(childPID==0){
		execvp(args[0],args);
		printf("%s\n", "File Not Resolved");
		exit(0);
	}

	else{
		charSequence=0;
		holderSequence=0;
		arraySequence=0;
		fileCounter=0;

		while(i<MAX_INPUT){ // A loop to empty the args string before waiting
			args[i]=0;
			i++;
			}
		i=0;
		wait();
	}
 }}