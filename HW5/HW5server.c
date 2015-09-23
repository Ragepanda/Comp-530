 //server
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
#define EXIT "exit\n"
  ServerSocket welcome_socket;
  Socket connect_socket;

  int main(int argc, char* argv[]){
	
	 ServerSocket welcome_socket;
	Socket connect_socket;
	
	char file_name[20]="tmp";
	char pid_string[10];
	int holder;
	int PID;
	pid_t child_pid, this_pid;
	this_pid = getpid();
	PID = getpid();
	
	int counter=0;
	char input_line[MAX_LINE];
	
	char *args[81];
	int i;
	int c;
	
	char *exit = "exit\n";
	
	 pid_t term_pid; /* pid_t is typedef for Linux process ID */ 
  int chld_status;

	snprintf(pid_string, 10,"%d",PID);
	
	strcat(file_name, pid_string);
	
	strcat(file_name, ".txt");
	
	printf("%s",file_name);
	
	FILE *fp;
	
	
	 if (argc < 2)
     {
      printf("No port specified\n");
      return (-1);
     }
	
  welcome_socket = ServerSocket_new(atoi(argv[1]));
  if (welcome_socket < 0){
      printf("Failed new server socket\n");
      return (-1);
  }
  else{
	printf("client connected");
  }
	 
	while(1==1){
		connect_socket = ServerSocket_accept(welcome_socket);
	
		printf("Connection made\n");
	while(1==1){
	printf("hit\n");
	
	printf("hit 2\n");
	  if (connect_socket < 0){
         printf("Failed accept on server socket\n");
         break;
      }
		counter = 0;
		for(i=0; i<MAX_LINE; i++){
			holder = Socket_getc(connect_socket);
			
		
		if (holder == EOF){
			   Socket_close(connect_socket);
                  return; 
            } 
			if(holder=='\n'){
				input_line[i]=holder;
				printf("%i", counter);
				break;
			}
			else{
				input_line[i]=holder;
				counter++;
				printf("%i",i);
			}
		
		}
		if(i==MAX_LINE)
			input_line[i-1]= '\0';
		counter = 0;
		
	
	//	printf("will this even happen?");
		child_pid = fork();
	
		if(child_pid == 0){
		printf("fork hit");
			i=0;
			args[i] = strtok(input_line," ");

			while(args[i]!=NULL){
				args[++i] = strtok(NULL," ");
			}
			printf("%s",args[0]);
			  fp = freopen(file_name, "w+", stdout);
			execvp(args[0],args);
			break;
		}
		
		else{
			printf("Fork hit");
			term_pid = waitpid(-1, &chld_status, WNOHANG);
		}
		
		counter=0;
		i=0;
		
		for(i=0; i<MAX_LINE; i++){
			input_line[i]=0;
			*args[i]=0;
		}
		
			fp=fopen(file_name,"r");
		
			holder = fgetc(fp);
		
			while(holder != EOF){
				Socket_putc(holder, connect_socket);
				holder = fgetc(fp);
			}
			for(i=0; i<5; i++){
				Socket_putc(exit[i],connect_socket);
			}
			remove(file_name);
		
	}	
	}
  }
  
  
  
  
  
  