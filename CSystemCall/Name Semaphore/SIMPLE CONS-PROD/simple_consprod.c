#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <semaphore.h>
#define N 10

void* consumer(void*);
void* producer(void*);
void thread_safe(char*, int);
void check_correct(sem_t*, sem_t*);
void create_sem(char*);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t* get;
sem_t* put;
int global_buffer[N];
int global_index = 0;

int main() {
 
 pthread_t cons_tid;
 pthread_t prod_tids[N];
 int thread_id[N];

 //semaphore initializing
 get = sem_open("get", 0);
 put = sem_open("put", 0);

 check_correct(get,put);

 for(int i=0; i<N;i++) {
 	thread_id[i] = i;
 	thread_safe("pthread_create", pthread_create(&prod_tids[i], NULL, producer, &thread_id[i]));
 }

 thread_safe("pthread_create", pthread_create(&cons_tid, NULL, consumer, NULL));

 thread_safe("pthread_join", pthread_join(cons_tid, NULL)); //will wait forever

}

void* consumer(void* arg) {
 
 int my_index = 0;
 int element;
 while(1) {
	sem_wait(get); //get is initially set to 0
	pthread_mutex_lock(&mutex); //acquire the mutex
	element = global_buffer[my_index%N];
	printf("Element at index %d is: %d\n",my_index%N, element);
	pthread_mutex_unlock(&mutex);
	sem_post(put); //tell the producer that there's 1 new empty spot to put in
	my_index++;
	
 }

}

void* producer(void* arg) {
 
 int my_id = *(int*)arg;
 while(1) {
 	sem_wait(put); //put is initally set to N (empty place into the buffer
	pthread_mutex_lock(&mutex);
	global_buffer[global_index%N] = my_id;
	global_index++;
	pthread_mutex_unlock(&mutex);
	sem_post(get); //tell the consumer that there's at least 1 element to consume
	
 }
}

void thread_safe(char* s_err, int n_err) {
 
 if(n_err != 0) {
 	fprintf(stderr, "%s: %d\n",s_err, n_err);
	exit(EXIT_FAILURE);
 }
}

void check_correct(sem_t* get, sem_t* put) {

 if(get != NULL && put != NULL) {
	printf("Semaphore correctly opened.\n\n");
 } else {
	fprintf(stderr,"SEM_OPEN: ERROR\n");
	exit(EXIT_FAILURE);
   }

}




