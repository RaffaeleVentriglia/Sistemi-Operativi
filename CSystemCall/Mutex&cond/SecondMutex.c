#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 20

void* thread_func(void*);

pthread_mutex_t mutex;
pthread_mutexattr_t attr;
int my_glob = 0;

int main() {

 pthread_t tid;
 int err;

 pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
 pthread_mutexattr_init(&attr);
 pthread_mutex_init(&mutex, &attr);
 
 if((err = pthread_create(&tid, NULL, thread_func, NULL)) != 0) {
	fprintf(stderr, "thread creation failed: %d", err);
	exit(EXIT_FAILURE);
 }

 for(int i=0; i<N; i++) {
	pthread_mutex_lock(&mutex);
	my_glob += 1;
	pthread_mutex_unlock(&mutex);
	printf("o");
	fflush(stdout);
	
 }

 pthread_join(tid, NULL);

 printf("\nMyglob = %d\n",my_glob);

 pthread_mutexattr_destroy(&attr);
 pthread_mutex_destroy(&mutex);

 return(0);

}

void* thread_func(void* arg) {
 int j;
 
 for(int i=0; i<N; i++) {
	pthread_mutex_lock(&mutex);
	j = my_glob;
	j++;
 	printf(".");
	fflush(stdout);
	my_glob = j;
	pthread_mutex_unlock(&mutex);
 }

}
