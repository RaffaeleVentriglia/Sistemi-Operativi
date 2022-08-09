#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

struct returning_struct {
 int termination_code;
 char termination_status[2];
};

typedef struct returning_struct ret_s;

void* Thread_Fun1(void*);
void* Thread_Fun2(void*);

int main() {

 ret_s* ret_struct;
 pthread_t tid[2]; //array of thread id
 void* t_status; //generic termination status
 char to_upper='a'; //char to uppercase
 int err, t_created=0;
 int* status;
 

 //creating first thread 
 if((err = pthread_create(&tid[0], NULL, Thread_Fun1, (void*)&to_upper)) != 0) {
	fprintf(stderr, "pthread_create errno: %d\n", err);
	exit(EXIT_FAILURE);
 } else t_created++;

 //creating second thread
 if((err = pthread_create(&tid[1], NULL, Thread_Fun2, NULL)) != 0) {
 	fprintf(stderr, "pthread_create errno: %d\n", err);
 	exit(EXIT_FAILURE);
 } else t_created++;

 printf("Main thread: executing.\n");
 
 //now wait them to finish and term status
 for(int i = 0; i<t_created; i++) {
	//join the son by the for (0 = first son, 1 = second son etc..)
 	if((err = pthread_join(tid[i], &t_status)) != 0) { //if can't join
		fprintf(stderr, "pthread_join errno: %d\n", err); //error
		exit(EXIT_FAILURE); //exit
	} else { //else
	 	ret_struct = (ret_s*)t_status; //assign the returning struct to the ret_struct defined before
 		printf("Thread %d: Termination Code: %d | Termination status: %s\n",i+1,ret_struct->termination_code,ret_struct->termination_status); //print info
  	  }

 }

 printf("Main thread: finished.\n");

}

//first function for the first thread
void* Thread_Fun1(void* arg) {
 //define a STATIC struct, otherwise the stack will be compromised
 static ret_s ret_struct1;
 char a[1];
 //copy the arg passed by into a
 strcpy(a, (char*)arg);

 printf("Thread 1: uppercasing %s\n",a);
 a[0]+=-32;
 printf("Thread 1: Done: %s - exiting now.\n", a);

 //updating the struct 
 ret_struct1.termination_code = 1;
 strcpy(ret_struct1.termination_status,"OK");

 //calling pthread and passing the struct as exit code
 pthread_exit((void*)&ret_struct1);
 
}

void* Thread_Fun2(void* arg) {
 //define a STATIC struct, otherwise the stack will be compromised
 static ret_s ret_struct2; 

 printf("Thread 2: just doing nothing, exiting now.\n");
 
 //update the struct
 ret_struct2.termination_code = 1;
 strcpy(ret_struct2.termination_status,"OK");

 //calling pthread and passing the struct as exit code
 pthread_exit((void*)&ret_struct2);

}

