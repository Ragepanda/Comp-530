
/*On my honor, I pledge that I have neither given nor received unauthorized aid on this assignment
Jonathan Woltz
Comp 530 -- Operating Systems
Professor Kevin Jeffay
Oct. 20, 2014

This program utilizes Linux pipelines to run three separate, concurrent processes, the goal of which are to 
take 80 characters from stdin, and output them, making sure to replace all newline characters with 
spaces and all '**' occurences with '^'. Process A takes in the characters from stdin and passes it to B, Process B does the logic handling
and character replacements, after which it hands it off to Process C. Process C prints out the characters it receives from B after 80 have 
been entered.
*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void aProcess(int write_descriptor);
void bProcess(int read_descriptor, int write_descriptor);
void cProcess(int read_descriptor);

int main(){

int abPipe[2];                // Processes are labeled 'A' 'B' 'C', abPipe connects A->B and bcPipe connect B->C
int bcPipe[2];
pid_t childPID;               // Use two separate PIDs for multiple forks
pid_t grandchildPID;
	
	pipe(abPipe);						// Preliminary Pipe and Fork
	childPID = fork();

        if(childPID>0){                   // Parent -- Process A           
		   close(abPipe[0]);              // close read descriptor 
            aProcess(abPipe[1]);             // Process A 
		}
//-------------------------------------------------------------------------------------------------------
        else if(childPID==0){                              // Child -- Processes B&C
             
					close(abPipe[1]); 						// Closes first pipe write descriptor
					pipe(bcPipe);							// Second pipe init
					grandchildPID=fork();                   // Second fork
					
					if(grandchildPID!=0){					// Parent -- Process C
						close(bcPipe[1]);					// Closes BC writer
						cProcess(bcPipe[0]);				// Initializes Process C
					}
					
					else{									// Child -- Process B
						close(bcPipe[0]);                       // Closes BC reader
						bProcess(abPipe[0],bcPipe[1]);          //	Initializes process B
					}
		}
  }

//------------------------------------------------------------------------------------------------------


void aProcess(int write_descriptor) {
    char c;
    do {
        c = getchar();
        write(write_descriptor, &c, sizeof(char));
    } while ( c != EOF);
}


void bProcess(int read_descriptor,int write_descriptor) {
    	
	char c;
	char holder;   // This value is used to store the asterick for '**' --> '^' replacement
	int count=1;	// This value is used to keep track of how many characters pass through to manage the edge case of an 80th asterick
		
		read(read_descriptor, &c, sizeof(char));
		
		while(c != EOF) {
			if(c=='\n'){         // Replaces newlines with an empty space
				c=' ';
			}
			
			if(c=='*'){           // Bracket for handling astericks
				holder==c;
				if(count!=80){		// Asterick replacement condition -- Making sure the asterick isn't the last character
					
					read(read_descriptor, &c, sizeof(char));
					count++;         //Incrementing count for the extra read, if it's the 80th character, the logic is handled after the asterick section
					
					if(c=='*'){       // Replacement if it's a '**'
						c='^';
					}
					
					else{             // Swaps c with the holder value then changes back to do a write for the first asterick
					
						if(c=='\n'){     // In case the extra read character was a newline, it needs to be replaced with a space
							c=' ';
						}
						
						holder=c; 
						c='*';
						write(write_descriptor,&c,sizeof(char));   // Writes the asterick to Process C
						c=holder;
					}
				}
			}
			
			
			write(write_descriptor,&c,sizeof(char));
			
			if(count==80)     // Checking to see if count needs to be looped back to 0
				count=0;
				
			read(read_descriptor, &c, sizeof(char));
			count++;           // incrementing the count 
		}
		printf("EOF\n");
			
}


void cProcess(int read_descriptor){
	char c;
	int count=0;   // Count is the number of characters that have been passed through, count is incremented by two for '^' characters
	int size=0;		// Size is the actual length of the array, and increments by 1 for '^' characters
	char array[80];  // Storage for the final output
	int i = 0;       // Used to increment the loop below
	
	
	while(c != EOF) {
		read(read_descriptor,&c,sizeof(char));
		array[size]=c;                                  // Dropping the character into the array
		size++; 										// Incrementing the length of the array
		if(c=='^')
			count=count+2;								// Incrementing the total character count by two for a '^'
		else
			count++;									// Increment by one for any other characters
			
		if(count==80){                                  // When count is 80, it's time to print!
			
			while(i<size){								// Loop that runs for how many elements are in the array: size
			putchar(array[i]);
			array[i]=0;
			i++;
			}
			
			
			putchar('\n');                              // Tacks on a newline character at the end
			count=0;									// Resets all tracking integers
			size=0;
			i=0;
		}
	}
}