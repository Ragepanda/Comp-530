 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <assert.h>
 #include "st.h"
 #include "semaphore.h"
 //#include "buffer.h"
 
 
 #define INPUT_LENGTH 79

 typedef struct{
	int depositCount;
	int withdrawCount;
	char *buffer;
} bufferedList;

void deposit(bufferedList *list, char p){
	if(list->depositCount==80)
		list->depositCount=0;
	list->buffer[list->depositCount]=p;
	list->depositCount=list->depositCount+1;
}

char withdraw(bufferedList *list){
	list->withdrawCount++;
	//printf("%i\n",list->withdrawCount);
	if(list->withdrawCount==79){
		list->withdrawCount=0;
		return(list->buffer[79]);
	}
	return(list->buffer[list->withdrawCount-1]);
}
 typedef struct {
    semaphore *mutex;           // Pointer to a mutex
    bufferedList list;     // Pointer to a shared string
} ThreadInit;

typedef struct {
    semaphore *mutex1;           // Pointer to a mutex
	semaphore *mutex2;
    bufferedList list1;     // Pointer to a shared string
	bufferedList list2;
} MidThreadInit;
 
void *inputProcess(void *state);
void *processingProcess(void *state);
void *outputProcess(void *state);

int main(){
	char inputString[80];
	char processingString[80];
	char outputString[80];
	st_init();
	
		 // Create and initialize a mutex to 1.
    semaphore mutex1;
    createSem(&mutex1, 1);
	
	semaphore mutex2;
	createSem(&mutex2, 1);


	bufferedList input_processing;
		input_processing.withdrawCount=0;
		input_processing.depositCount=0,
		input_processing.buffer=processingString;
	
	
	bufferedList processing_output;
		 processing_output.withdrawCount= 0;
		 processing_output.depositCount= 0;
		 processing_output.buffer= outputString;
	
	
	ThreadInit inputThread;
		inputThread.mutex= &mutex1;
		inputThread.list= input_processing;
	
	
	MidThreadInit midThread;
		midThread.mutex1= &mutex1;
		midThread.mutex2= &mutex2;
		midThread.list1= input_processing;
		midThread.list2= processing_output;
	
	
	ThreadInit outputThread;
		outputThread.mutex= &mutex2;
		outputThread.list= processing_output;
	
	
	 if (st_thread_create(inputProcess, &inputThread, 0, 0) == NULL) {
        perror("st_thread_create failed for thread 1");
        exit(EXIT_FAILURE);
    }

	
	 if (st_thread_create(processingProcess, &midThread, 0, 0) == NULL) {
        perror("st_thread_create failed for thread 2");
        exit(EXIT_FAILURE);
    }
	
	
	 if (st_thread_create(outputProcess, &outputThread, 0, 0) == NULL) {
        perror("st_thread_create failed for thread 3");
        exit(EXIT_FAILURE);
    }

	st_thread_exit(NULL);
	}
	
void *inputProcess(void *state){
	printf("%s\n","input");
	int inputSequence=0;
	int inputRuns=0;
	char inputStream [80];
	ThreadInit *list = state;
	
	while(1==1){
	
		inputStream[inputSequence]=getchar();
		
		down(list->mutex);
			if(inputStream[inputSequence]=='\n')
				deposit(&list->list,' ');
			deposit(&list->list,inputStream[inputSequence]);
			if(inputRuns==1){
				inputSequence=inputSequence+1;
				inputStream[inputSequence]=getchar();
				deposit(&list->list,inputStream[inputSequence]);
			}
			if(inputSequence==79){
				deposit(&list->list,inputStream[inputSequence]);
				deposit(&list->list,inputStream[inputSequence+1]);
				deposit(&list->list,EOF);
			}
		up(list->mutex);
		//printf("%c INPUT--- Run#%i\n",inputStream[inputSequence],inputSequence);
		
		
		inputSequence=inputSequence+1;
		inputRuns++;
		
	}
}

void *processingProcess(void *state){
	printf("%s\n","processing");
	MidThreadInit *listSet = state;
		int inputSequence=0;
		int starterValue = 0;
		char inputStream [80];
	
	while(1==1){
		if(starterValue==0){
			down(listSet->mutex1);
				inputStream[inputSequence]=withdraw(&listSet->list1);
			up(listSet->mutex1);
			//printf("PROCESSING #%i -- %c\n",inputSequence,inputStream[inputSequence]);
			//down(listSet->mutex2);
			//	deposit(&listSet->list2,inputStream[inputSequence]);
			//up(listSet->mutex2);
			inputSequence=inputSequence+1%79;
			starterValue++;
		}
		else{
		down(listSet->mutex1);
			inputStream[inputSequence]=withdraw(&listSet->list1);
			inputStream[inputSequence+1]=withdraw(&listSet->list1);
		up(listSet->mutex1);
		//printf("PROCESSING #%i -- %c\n",inputSequence,inputStream[inputSequence]);
		//printf("PROCESSING #%i -- %c\n",inputSequence+1,inputStream[inputSequence+1%79]);
		
		
		
			//printf("%c PROCESSING -- Run #%i\n",inputStream[inputSequence],inputSequence);
			//printf("#%i-- %c\n",listSet->list1.withdrawCount, inputStream[inputSequence]);
			
		// ^ logic
			if(starterValue==1){
				down(listSet->mutex2);
					deposit(&listSet->list2,inputStream[inputSequence-1]);
					deposit(&listSet->list2,inputStream[inputSequence]);
					deposit(&listSet->list2,inputStream[inputSequence+1%79]);
				up(listSet->mutex2);
		
				inputSequence=inputSequence+1%79;
				inputSequence=inputSequence+1%79;
				starterValue++;
			}
			
			else{
				down(listSet->mutex2);
					deposit(&listSet->list2,inputStream[inputSequence]);
					deposit(&listSet->list2,inputStream[inputSequence+1%79]);
				up(listSet->mutex2);
		
				inputSequence=inputSequence+1%79;
				inputSequence=inputSequence+1%79;
			
			}
			
		}
		
	}
}


void *outputProcess(void *state){
	int inputSequence=0;
	char inputStream [80];
	int initializer =0;
	int i;
	printf("%s","outputting\n");
	ThreadInit *list = state;
	
	
	
	while(1==1){
	

		down(list->mutex);
			if(initializer<1){
				initializer++;
				printf("init\n");
			}
			else{
				inputStream[inputSequence]=withdraw(&list->list);
			}
		up(list->mutex);
		if(inputSequence==3)
			inputStream[inputSequence+1]=withdraw(&list->list);
		if(inputStream[inputSequence]==EOF){
			for(i=0; i<inputSequence; i++){
			printf("%c",inputStream[i]);
			}
		inputSequence==0;
		}
		//printf("OUPUT RUN#%i -- %c\n",inputSequence,inputStream[inputSequence]);
		printf("%c",inputStream[inputSequence]);
		inputSequence=inputSequence+1%80;
	}
}