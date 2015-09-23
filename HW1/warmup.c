//C program John Woltz
// Comp 530
// On my honor, I pledge that I have neither given nor received unauthorized aid on this assignment
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #define INPUT_LENGTH 79
 #define FALSE 0
 #define TRUE 1
 
 main(){
 char inputStream [INPUT_LENGTH]; // Amount of elements that I want to be held at a given time
 char inputHolder;       // Used to hold a character value in the event of needing to read two characters for a '**' use case
 int charCalled = FALSE; // Boolean for if a getchar() method needs to be called or not
 int inputSequence = 0; // Used to increment up the array

 
	while(TRUE){
		while(inputSequence <=INPUT_LENGTH){
			if(charCalled == FALSE){                                        // If/Else Branch for determining whether another getchar() is necessary
				inputStream[inputSequence]=getchar();
			}
			else{
				inputStream[inputSequence] = inputHolder;
				charCalled = FALSE;
			}
			if (inputStream[inputSequence] == EOF)                        // reads if the last input was an EOF
				exit(1);
			
			if(inputStream[inputSequence]== '\n'){                         // Check for new line characters
				inputStream[inputSequence] = ' ';
			}
				
			if(inputStream[inputSequence] == '*'){                            // Check for asterisks
				inputHolder=getchar();
				if(inputHolder =='*'){
					inputStream[inputSequence] = '^';
				}
				else{                                            // Use case of at 80th element and there are two
					charCalled = TRUE;                               // Setting boolean so that the getchar() method is blocked
				}
			}	
			inputSequence++;	// Increment inputSequence at the end of the loop
	}	
	
		printf("%s\n",inputStream);
		inputSequence = 0;                                            // Resetting the inputSequences for the next 80 characters
 }
 }