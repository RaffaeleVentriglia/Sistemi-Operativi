#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define N 10

void* tester(void*);
void* setter(void*);
void thread_safe(char*, int);

int full = 0;
int array[N] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main() {

 srand(time(NULL));

 pthread_t tid;
 thread_safe("PTHREAD_CREATE", pthread_create(&tid, NULL, tester, NULL));
 thread_safe("PTHREAD_CREATE", pthread_create(&tid, NULL, setter, NULL));

 thread_safe("PTHREAD_JOIN", pthread_join(tid, NULL));

}

void thread_safe(char* s_err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "THREAD ERROR: %s: %d\n",s_err, n_err);
	exit(EXIT_FAILURE);
 }

}

void* tester (void* arg) {
 
 pthread_mutex_lock(&mutex);
 while(full != (N*3)-1) 
	pthread_cond_wait(&cond,&mutex);

 pthread_mutex_unlock(&mutex);
 fflush(stdout);
 printf("tester array: ");
 for(int i = 0; i<N; i++) {
 	printf("%d ", array[i]);

 }
 printf("\n");

}

void* setter (void* arg) {

 for(int i = 0; i<N*3; i++) {
	array[i%N] = rand()% 10+1;
	full++;

 }

 pthread_cond_signal(&cond);
 fflush(stdout);
 printf("setter array: ");
 for(int i = 0; i<N; i++) printf("%d ",array[i]);  
 fflush(stdout);
}
