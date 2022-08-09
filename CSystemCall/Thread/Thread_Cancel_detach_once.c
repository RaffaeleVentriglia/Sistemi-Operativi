#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//NB: this program will use pthread_once, pthread_attr_t, and pthread_cancel functions related.

void* thread_fun(void*);
void thread_init_fun(void);

pthread_once_t init = PTHREAD_ONCE_INIT; //global flag for pthread_once
int N = 5;
int globtid = 0;

int main() {

 pthread_t tids[N]; //array of tids (N=5)
 pthread_attr_t attr; //structure of thread attribute
 int ret_id;
 int err; //variable to take track of eventual errors
 void* retval;

 printf("MASTER THREAD ID %d HERE\n",0);
 
 //create N thread
 for(int i = 0; i<N; i++) {
 	

	//if not the las iteration
	if((err = pthread_create(&tids[i], NULL, thread_fun, NULL)) != 0) {
		fprintf(stderr, "PTHREAD_CREATE: %d\n", err);
		exit(EXIT_FAILURE);
        }

 }
 
 puts("");

 //now wait for the thread to finish
 for(int i = 0; i<N; i++) {
	if((err = pthread_join(tids[i], &retval)) == 0) {
		ret_id = *(int*)retval;
		printf("THREAD ID %d RETURNED.\n", ret_id);
	} else printf("THREAD FOUND NOT JOINABLE.\n");
 }

 printf("MASTER THREAD FINISHED.\n");

}


void* thread_fun(void* arg) {

 int mytid = globtid++;

 pthread_once(&init, thread_init_fun); //doing one time this function

 printf("THREAD ID %d FINISHING.\n",globtid);

 pthread_exit((void*)&mytid);
 
}


void thread_init_fun(void) {

 printf("INITIALIZING DONE ONCE, EXITING.\n");

}
