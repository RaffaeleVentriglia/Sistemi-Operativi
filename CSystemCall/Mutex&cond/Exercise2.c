#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_adder(void*);
void* thread_checker(void*);
void thread_safe(char*, int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int sum_value = 0;
int sum_counter = 0;
int notified = 0;
int max_limit;


int main(int argc, char* argv[]) {

 srand(time(NULL));
 max_limit = atoi(argv[1]);
 pthread_t tids[3];
 
 thread_safe("pthread_create", pthread_create(&tids[0], NULL, thread_adder, NULL));
 thread_safe("pthread_create", pthread_create(&tids[1], NULL, thread_adder, NULL));
 thread_safe("pthread_create", pthread_create(&tids[2], NULL, thread_checker, NULL));
 thread_safe("pthread_join", pthread_join(tids[2], NULL)); //wait for the checker, the last thread executed

}

void* thread_adder(void* arg) {
 
 int rndelem;
 while(1) {
 	for(int i=0; i<1000; i++) {
		rndelem = (rand()%(1000 + 1 - 0) + 0);

		pthread_mutex_lock(&mutex);
		if(sum_counter >= max_limit && notified == 0) {
			pthread_cond_signal(&cond);
			notified = 1;
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		} else if(sum_counter >= max_limit && notified == 1){
		 	pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);		  	
		  }
		sum_value+=rndelem;
		sum_counter++;
		pthread_mutex_unlock(&mutex);
		
	}
 }

}

void* thread_checker(void* arg) {

 pthread_mutex_lock(&mutex);
 while(sum_counter < max_limit) {
	pthread_cond_wait(&cond, &mutex);
 }

 pthread_mutex_unlock(&mutex);
 
 printf("THREAD CHECKER: Max limit reached - total sum: %d\n", sum_value);
 pthread_exit(NULL);

}

void thread_safe(char* s_err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "%s : %d", s_err, n_err);
	exit(EXIT_FAILURE);
 }

}
