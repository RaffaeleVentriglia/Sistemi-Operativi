#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* writer(void*);
void* reader(void*);
void* waiter(void*);
void thread_safe(char*, int);
void file_safe(char*, int);

int N,gfd;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t* sem_writer;
sem_t* sem_reader;
sem_t* sem_waiter;

int main(int argc, char* argv[]) {

 pthread_t twriter, treader, twaiter;
 N = atoi(argv[1]);

 if(N < 13 || N > 27) {
	printf("Usage: ./filename N (13<=N<=27)\n");
	exit(EXIT_FAILURE);
 }
 
 //open file
 gfd = open("FILE", O_CREAT | O_TRUNC | O_RDWR, 0666);
 
 if(gfd == -1) {
 	perror("OPEN");
	exit(EXIT_FAILURE);
 } 


 //init semaphore
 sem_writer = sem_open("sem_writer", O_CREAT | O_EXCL, 0666, 1);
 sem_reader = sem_open("sem_reader", O_CREAT | O_EXCL, 0666, 0);
 sem_waiter = sem_open("sem_waiter", O_CREAT | O_EXCL, 0666, 0);

 thread_safe("create writer", pthread_create(&twriter, NULL, writer, NULL));
 thread_safe("create reader", pthread_create(&treader, NULL, reader, NULL));
 thread_safe("create waiter", pthread_create(&twaiter, NULL, waiter, NULL));

 thread_safe("join writer", pthread_join(twriter, NULL));
 thread_safe("join reader", pthread_join(treader, NULL));
 thread_safe("join waiter", pthread_join(twaiter, NULL));

 sem_close(sem_writer);
 sem_close(sem_reader);
 sem_close(sem_waiter);
 sem_unlink("sem_writer");
 sem_unlink("sem_reader");
 sem_unlink("sem_waiter");
 if(fork()==0) {
 	execlp("rm", "rm", "FILE", NULL);
 }

 return(0);

}

void* writer(void* arg) {
 
 int to_write = 0;
 while(to_write != N) {
	sem_wait(sem_writer);
	pthread_mutex_lock(&mutex);
	file_safe("write", write(gfd, &to_write, sizeof(int)));
	lseek(gfd, 0, SEEK_SET);
	pthread_mutex_unlock(&mutex);
	sem_post(sem_reader);
	to_write++;
	sleep(1);
 }

 sem_post(sem_waiter);
 pthread_exit(NULL);

}

void* reader(void* arg) {

 int to_read = 0;
 while(to_read != N) {
	sem_wait(sem_reader);
	pthread_mutex_lock(&mutex);
	file_safe("read", read(gfd, &to_read, sizeof(int)));
 	lseek(gfd, 0, SEEK_SET);
	printf("Numero letto: %d\n",to_read);
	pthread_mutex_unlock(&mutex);
	sem_post(sem_writer);
	sleep(1);
 }
 
 pthread_exit(NULL);

}

void* waiter(void* arg) {

 sem_wait(sem_waiter);
 printf("Finito. Thread TID: %lu\n", pthread_self());
 pthread_exit(NULL);

}

void thread_safe(char* serr, int nerr) {
 
 if(nerr != 0) {
	fprintf(stderr, "%s : %d",serr,nerr);
	exit(EXIT_FAILURE);
 }

}

void file_safe(char* serr, int nerr) {

 if(nerr == -1) {
	perror(serr);
	exit(EXIT_FAILURE);
 }

}
