#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>



void* factorial_writer(void*);
void* factorial_reader(void*);
void thread_safe(int nerr, char* serr);

sem_t* sem_reader;
sem_t* sem_writer;
int N;
int Factorial;
int starting_num;

int main(int argc, char* argv[]) {

 srand(time(NULL));
 pthread_t writertid, readertid;
 
 if(argc!=2) {
	printf("Usage: ./filename <N>\n");
	exit(EXIT_FAILURE);	
 } else N = atoi(argv[1]);

 if((sem_writer = sem_open("sem_writer", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "SEM_WRITER: CAN'T CREATE\n");
	exit(EXIT_FAILURE);
 }

 if((sem_reader = sem_open("sem_reader", O_CREAT | O_EXCL, 0666, 0)) == NULL) {
	fprintf(stderr, "SEM_WRITER: CAN'T CREATE\n");	
	exit(EXIT_FAILURE);	
 }

 //CREATION THREAD
 thread_safe(pthread_create(&writertid, NULL, factorial_writer, NULL), "writer thread");
 thread_safe(pthread_create(&readertid, NULL, factorial_reader, NULL), "reader thread");

 //Thread waiting
 thread_safe(pthread_join(writertid, NULL), "writer join");
 thread_safe(pthread_join(readertid, NULL), "reader join");

 sem_close(sem_reader); 
 sem_close(sem_writer);
 sem_unlink("sem_reader");
 sem_unlink("sem_writer");

}

void* factorial_writer(void* arg) {

 int fact = 1;
 int rand_num;

 for(int i=0; i<N; i++) {
	sem_wait(sem_writer);
	rand_num = rand()% 10+0;
	for(int j=1; j<=rand_num; j++) {
		fact = fact*j;
	}
	starting_num = rand_num;
	Factorial = fact;
	fact = 1;
	sem_post(sem_reader);
	sleep(1);
 }

 pthread_exit(NULL);	
 
}

void* factorial_reader(void* arg) {

 int to_read;
 int start_num;
 
 for(int i=0; i<N; i++) {
	sem_wait(sem_reader);
	to_read = Factorial;
	start_num = starting_num;	
	printf("Factorial of %d is: %d\n",start_num, to_read);
	sem_post(sem_writer);
	sleep(1);
 }

 pthread_exit(NULL);
 
}

void thread_safe(int nerr, char* serr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n", serr,nerr);
	exit(EXIT_FAILURE);
 }

}
