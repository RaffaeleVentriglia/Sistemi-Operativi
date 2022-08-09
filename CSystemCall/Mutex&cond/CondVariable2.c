#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 10

void* simple_producer(void*);
void* simple_consumer(void*);
void thread_safe(char*, int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int consumer_index = 0;
int empty = N;
int full = 0;
int buffer[N] = {0}; //initializing N positions to 0

int main() {

 srand(time(NULL));

 pthread_t tids [2];

 thread_safe("PTHREAD CREATE", pthread_create(&tids[0], NULL, simple_consumer, NULL));
 thread_safe("PTHREAD CREATE", pthread_create(&tids[1], NULL, simple_producer, NULL));

 thread_safe("PTHREAD JOIN", pthread_join(tids[1], NULL));

}

void* simple_producer(void* arg) {
 while(1) {
	pthread_mutex_lock(&mutex);
	while(empty == 0) { //if the buffer is full, the producer needs to wait.
		printf("PRODUCER WAITING.\n");
		pthread_cond_wait(&cond, &mutex); //wait until the buffer is empty again

	}
	
	buffer[full%N] = rand()% 10+1; //produce a random element
	full++; //increase the index
	empty--; //decrease the emptyness
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond); //inform the producer that there's at least 1 element to consume	

 }
}

void* simple_consumer(void* arg) {
 int element;
 while(1) {
	pthread_mutex_lock(&mutex);
	while(full == 0) {
		pthread_cond_wait(&cond, &mutex); //wait until the buffer is at least 1 full
	}
	
	element = buffer[consumer_index%N]; //consume the element starting from position 0
	consumer_index++; 
	
	
	full--; //decrease the fullness 
	empty++; //increase the emptyness
	
	printf("Empty % d - Full %d - Element %d consumed at index: %d\n",empty, full, element, consumer_index);

	pthread_mutex_unlock(&mutex);
	if(empty == N) pthread_cond_signal(&cond);

	
 }
}

void thread_safe(char* s_err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "%s : %d", s_err, n_err);
	exit(EXIT_FAILURE);
 }

}
