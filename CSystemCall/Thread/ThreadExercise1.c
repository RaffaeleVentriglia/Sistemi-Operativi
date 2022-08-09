#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void ERR_SYS(char*,int);
void* thread_fun (void*);

int globalvar = 0;

int main(int argc, char* argv[]) {

 srand(time(NULL)); //seed initializing
 int N = atoi(argv[1]);
 int err;
 int t_ids[N-1];
 pthread_t tids[N-1];
 




 //creating N threads
 for(int i = 0; i<N; i++) {
 	t_ids[i] = i; 
	if((err = pthread_create(&tids[i], NULL, thread_fun, &t_ids[i])) != 0)
		ERR_SYS("pthread_create", err);
 }

 //waiting N thread
 for(int i = 0; i<N; i++) {
	if((err = pthread_join(tids[i], NULL)) != 0) 
		ERR_SYS("pthread_join", err);
 }

 printf("MASTER THREAD FINISHED.\n");
 return(0);

}

void ERR_SYS(char* strerror, int errnumber) {

 fprintf(stderr, "%s: %d", strerror, errnumber);
 exit(EXIT_FAILURE);

}

void* thread_fun(void* arg) {

 int towait = rand() % 10 + 1;
 printf("Thread %d going to sleep for: %d secs.\n",*(int*)arg, towait);
 sleep(towait);
 globalvar++; 
 printf("Thread %d increased globalvar value: %d\n",*(int*)arg, globalvar);
 pthread_exit(NULL);

}
