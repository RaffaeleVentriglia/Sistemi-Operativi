#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define N 10

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --Big note: before we init a memory semaphore with SEM_INIT function, we need to do 2 things first:										  --
// --1) if we declare some sem_t* (sem_t pointers) the function expect that this pointers are already memory-allocated. so initialize it with a malloc or other stuff		  --
// --2) if we won't declare sem_t as pointer (sem_t vanilla type) we need to pass the memory address of the object to the sem_init function. because memory is already allocated. --
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void* consumer (void* arg);
void* producer (void* arg);
void thread_safe(char*, int);
void initialize_sem(void);


int global_buffer[N];
int global_index = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t* get;
sem_t* put;

int main() {

 pthread_t producers[N];
 pthread_t cons;

 initialize_sem();
 sem_init(get, 0, 0);
 sem_init(put, 0, N);

 for(int i=0; i<N; i++) {
	thread_safe("PTHREAD_CREATE", pthread_create(&producers[i], NULL, producer, NULL));
 }

 thread_safe("PTHREAD_CREATE", pthread_create(&cons, NULL, consumer, NULL));

 //now wait at least one to not exit
 thread_safe("PTHREAD_JOIN", pthread_join(cons, NULL));
 return(0);

}

void* consumer (void* arg) {

 int my_index = 0;
 while(1) {
	sem_wait(get);
	pthread_mutex_lock(&mutex);
	if(my_index != global_buffer[my_index%N]) { //if the current element is not equal to the index value
		printf("Wrong element at index %d - element value: %d\n",my_index%N, global_buffer[my_index%N]);
	}
	pthread_mutex_unlock(&mutex);
	sem_post(put);
	my_index++;
 }
}

void* producer (void* arg) {

 while(1) {
	sem_wait(put);
	pthread_mutex_lock(&mutex);
	global_buffer[global_index%N] = global_index%N;
	global_index++;
	pthread_mutex_unlock(&mutex);
	sem_post(get);
 }
}

void thread_safe(char* s_err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "%s : %d", s_err, n_err);
	exit(EXIT_FAILURE);
 }
 
}

void initialize_sem() {

 get = malloc(sizeof(sem_t));
 put = malloc(sizeof(sem_t)); 

}
