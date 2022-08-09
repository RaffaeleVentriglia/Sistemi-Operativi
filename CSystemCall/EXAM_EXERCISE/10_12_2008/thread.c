#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_sum(void*);
void* thread_waiter(void*);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

sem_t* sem_increase;

int limit;
int counter = 0;
int notified = 0;

int main(int argc, char* argv[]) {

 srand(time(NULL));

 if(argc!=2) {
	printf("Usage ./filename <N>\n");
	exit(EXIT_FAILURE);	
 } else limit = atoi(argv[1]);

 pthread_t thread_sum1;
 pthread_t thread_sum2;
 pthread_t thread_waiter1;

 if((sem_increase = sem_open("sem_increase", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	printf("SEM_OPEN: ERROR\n");
	exit(EXIT_FAILURE);	
 }

 if(pthread_create(&thread_sum1, NULL, thread_sum, NULL) != 0) {
	printf("thread sum 1 error\n");
	exit(EXIT_FAILURE);	
 }
 
 if(pthread_create(&thread_sum2, NULL, thread_sum, NULL) !=0) {
	printf("thread sum 2 error\n");
	exit(EXIT_FAILURE);
 }

 if(pthread_create(&thread_waiter1, NULL, thread_waiter, NULL) !=0) {
	printf("thread waiter error\n");
	exit(EXIT_FAILURE);
 }

 if(pthread_join(thread_sum1, NULL) != 0) {
	printf("join thread sum 1 error\n");
	exit(EXIT_FAILURE);
 }

  if(pthread_join(thread_sum2, NULL) != 0) {
	printf("join thread sum 2 error\n");
	exit(EXIT_FAILURE);
 }


  if(pthread_join(thread_waiter1, NULL) != 0) {
	printf("join thread waiter error\n");
	exit(EXIT_FAILURE);
 }

 sem_close(sem_increase);
 sem_unlink("sem_increase");


}


void* thread_sum(void* arg) {

 int num1,num2;
 int sum;
 for(int i=0; i<limit; i++) {
	sem_wait(sem_increase);
	if(counter >= limit) {
		if(notified == 1) {
			printf("t: %ld -> already notified\n",pthread_self());
			sem_post(sem_increase);
			pthread_exit(NULL);
		} else {
			notified = 1;
			printf("t: %ld -> notifying..\n",pthread_self());
			sem_post(sem_increase);
			pthread_mutex_lock(&mutex);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);				
		}
	}
	sem_post(sem_increase);
	num1 = rand()%100+1;
	num2 = rand()%100+1;
	sum = num1+num2;
 	sem_wait(sem_increase);
	if
		(counter == limit) { }
	else
		counter++;
	sem_post(sem_increase);
 }


}

void* thread_waiter(void* arg) {

 pthread_mutex_lock(&mutex);
 while(counter < limit) {
	pthread_cond_wait(&cond, &mutex);
 }

 printf("COUNTER %d is equal to LIMIT %d... EXITING..\n",counter, limit);
 pthread_mutex_unlock(&mutex);
 pthread_exit(NULL);

}



