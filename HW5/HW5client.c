// Client
//C program John Woltz
// Comp 530
// On my honor, I pledge that I have neither given nor received unauthorized aid on this assignment

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Socket.h"
#define MAX_LINE 80
 
 main(int argc, char* argv[]){
 Socket connect_socket;
int counter = 0;
int socket_Check = 0;
char holder;
char string_input[MAX_LINE];
int i;
	
	connect_socket = Socket_new(argv[1], atoi(argv[2]));
	if (connect_socket < 0)
      {
       printf("Failed to connect to server\n");
       return (-1);
      }
	  else{
		printf("You're in");
	  }
	printf("%s","%");
	while ((fgets(string_input, sizeof(string_input), stdin) != NULL)){
		counter = strlen(string_input) + 1;
		
		for(i=0; i<strlen(string_input)+1; i++){                                      // Sending out info to socket
			holder = string_input[i];
			socket_Check = Socket_putc(holder, connect_socket);
			if(socket_Check==EOF||holder==EOF){
				printf("Socket_putc EOF or error\n");
				Socket_close(connect_socket);
				exit(-1);
			}
		}
		
		
		counter = 0;
		while(1==1){                                               // Receiving info from server
			holder = Socket_getc(connect_socket);
			if (holder == EOF){
				printf("Socket_getc EOF or error\n");             
				Socket_close(connect_socket);
				exit (-1);  /* assume socket problem is fatal, end client */
			}
				
				
			if(holder == '\n'){
				string_input[counter]=holder;
				if(strncmp(string_input,"exit\n",100)==0){
					counter=0;
					for(i=0;i<MAX_LINE;i++){
						string_input[i]=0;
					}	
					break;
				}
				else{
					printf("%s",string_input);
					
					for(i=0;i<MAX_LINE;i++){
						string_input[i]=0;
					}	
				}
			}
			else{
			string_input[counter]=holder;
			}
		}
		printf("%");
		
		
		
		
	}
 
 }