#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_CHARS 20

struct queue_msg {
 int msg_q;
 struct queue_msg* next_msg;
};

typedef struct queue_msg q_msg;
static  struct queue_msg EMPTY_STRUCT;

void* thread_reader(void*);
void* thread_sender(void*);
void safe(char*, int);

q_msg* my_queue; //initially our global pointer queue is empty
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {

 pthread_t tid;

 safe("pthread create", pthread_create(&tid, NULL, thread_reader, NULL));
 safe("pthread create", pthread_create(&tid, NULL, thread_sender, NULL));

 safe("pthread join", pthread_join(tid,NULL));

 return(0);

}


void* thread_reader(void* arg) {

 while(1) {
	
	
	pthread_mutex_lock(&mutex); 
	while(my_queue==NULL) {
		printf("R: queue empty. waiting for a msg.\n");
		pthread_cond_wait(&cond, &mutex);
		printf("R: queue signaled full.\n");
	}

	printf("Message in the queue: %d\n",my_queue->msg_q);
	my_queue->next_msg = NULL;
	my_queue = my_queue->next_msg;

	pthread_mutex_unlock(&mutex);
 }
 
}

void* thread_sender(void* arg) {

 q_msg* sender_queue;
 int msg;

 while(1) {
	
	printf("S: insert msg into the queue: ");
	scanf("%d",&msg);
	
	pthread_mutex_lock(&mutex);
	sender_queue = &EMPTY_STRUCT;
	sender_queue->msg_q = msg;
	my_queue = sender_queue;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	sleep(1);

 }
}

void safe(char* err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "%s: %d",err, n_err);
	exit(EXIT_FAILURE);
 }

}
