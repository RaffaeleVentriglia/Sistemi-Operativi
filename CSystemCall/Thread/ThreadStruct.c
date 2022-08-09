#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct parameters {
 pthread_t father_tid;
 int IO_count;
 char to_print;
};

typedef struct parameters par;

void* Thread_Fun(void*);

int main() {

 par par_t1;
 pthread_t tid;
 int err; //to memorize the error, just in case

 par_t1.father_tid = pthread_self(); //passing the TID of the father
 par_t1.IO_count = 5; //times to print
 par_t1.to_print = 'A'; //char to print 

 //creating thread
 if((err = pthread_create(&tid, NULL, Thread_Fun, (void*)&par_t1)) != 0) { //if the creation was not made
 	fprintf(stderr, "cant create thread - error number %d.\n", err); //print error
	exit(EXIT_FAILURE); //and exit
}

 printf("Main thread here.\n");
 sleep(1); //else wait for thread finishing - later we'll use pthread_join to wait the son
 printf("Main thread finishing.\n");
 return(0); //return

}

void* Thread_Fun(void* arg) {

 par* thread_parameters = (par*)arg; //we need a pointer cast cause we passed par_t1 as a (void*) address into pthread create, so now we need an address too.
 
 printf("Thread created - FATHER TID: %lu\n",thread_parameters->father_tid);
 printf("Now printing %c for %d times\n", thread_parameters->to_print, thread_parameters->IO_count);
 for(int i = 0; i < thread_parameters->IO_count; i++) {
	printf("%c\n",thread_parameters->to_print);
 }



}
