#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_writer(void*);
void* thread_reader(void*);
void* thread_waiter(void*);
void thread_safe(char*, int);
void is_prime(int);

int gfd; 
int N;

sem_t* sem_writer;
sem_t* sem_reader;
sem_t* sem_waiter;

int main(int argc, char* argv[]) {

 if(argc!=2) {
	printf("usage: ./filename <N = 7<=N<=29>\n");
	exit(EXIT_FAILURE);
 } else { 
	N = atoi(argv[1]);
	if(N > 29 || N < 7) {
		printf("usage: ./filename <N = 7<=N<=29>\n");	
		exit(EXIT_FAILURE);	
 	}
	is_prime(N);
 }
 
 pthread_t tids[3];

 if((sem_writer = sem_open("sem_writer", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "sem open writer");
	exit(EXIT_FAILURE);
 }	

 if((sem_reader = sem_open("sem_reader", O_CREAT | O_EXCL, 0666, 0)) == NULL) {
	fprintf(stderr, "sem open reader");
	exit(EXIT_FAILURE);
 }	

 if((sem_waiter = sem_open("sem_waiter", O_CREAT | O_EXCL, 0666, 0)) == NULL) {
	fprintf(stderr, "sem open waiter");
	exit(EXIT_FAILURE);
 }	

 if((gfd = open("FILE", O_CREAT | O_TRUNC | O_EXCL | O_RDWR, 0666)) == -1) {
	perror("open");
	exit(EXIT_FAILURE);
 }

 thread_safe("pthread create writer", pthread_create(&tids[0], NULL, thread_writer, NULL));
 thread_safe("pthread create reader", pthread_create(&tids[1], NULL, thread_reader, NULL));
 thread_safe("pthread create waiter", pthread_create(&tids[2], NULL, thread_waiter, NULL));

 thread_safe("pthread join writer", pthread_join(tids[0], NULL));
 thread_safe("pthread join reader", pthread_join(tids[1], NULL));
 thread_safe("pthread join waiter", pthread_join(tids[2], NULL));

 sem_close(sem_writer);
 sem_close(sem_reader);
 sem_close(sem_waiter);
 sem_unlink("sem_writer");
 sem_unlink("sem_reader");
 sem_unlink("sem_waiter");
 if(fork()==0) {
	execlp("rm", "rm", "./FILE", NULL);
 }
 close(gfd);
 


}

void* thread_writer(void* arg) {

 int to_write;
 for(int i=1; i<=N; i++) {
	to_write = i;
	sem_wait(sem_writer);
	if(write(gfd, &to_write, sizeof(int)) < 0) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	if(lseek(gfd, 0, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);	
	}
	sem_post(sem_reader);
	sleep(1);
 }
 
 pthread_exit(NULL);

}

void* thread_reader(void* arg) {

 int to_read;
 for(int i=1; i<=N; i++) {
	sem_wait(sem_reader);
	if(read(gfd, &to_read, sizeof(int)) < 0) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	if(lseek(gfd, 0, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);
	} 
	printf("T-R: %d\n", to_read);
	sem_post(sem_writer);
	sleep(1);
 }

 sem_post(sem_waiter);
 pthread_exit(NULL);

}

void* thread_waiter(void* arg) {

 sem_wait(sem_waiter);
 printf("Operazioni concluse, arrivederci dal thread tid: %ld\n",pthread_self());
 sleep(5);
 pthread_exit(NULL);
 

}

void thread_safe(char* serr, int nerr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n",serr, nerr);
	exit(EXIT_FAILURE);
 
 }

}

void is_prime(int n) {

 for(int i=2; i<n/2; i++) {
	if(n % 2 == 0) {
		printf("%d non e' primo.\n",n);
		exit(EXIT_FAILURE);
	}
 }
}
