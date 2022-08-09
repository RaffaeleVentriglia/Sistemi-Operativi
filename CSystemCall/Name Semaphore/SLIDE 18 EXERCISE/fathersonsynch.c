#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* father_thread(void*);
void* son_thread(void*);
void thread_safe(char*, int);
int file_safe(char*, int);

int gfd;
int N;
int N_written = 0; //index to know when the N limit is reached.

sem_t* acquire_write_right;
sem_t* increase_written_count;

int main(int argc, char* argv[]) {

 pthread_t father, son;
 int fd;
 
 if(argc!=2) {
 	printf("USAGE: ./fathersonsynch <MAX NUMBER TO WRITE>\n");
	exit(EXIT_FAILURE);
 }
 

 N = atoi(argv[1]);

 gfd = fd = file_safe("OPEN", open("TEMP", O_CREAT | O_RDWR | O_TRUNC | O_EXCL, 0666)); //creating the file
 
 acquire_write_right = sem_open("acquire_write_right", O_CREAT | O_EXCL, 0666, 1); //creating the semaphore
 increase_written_count = sem_open("increase_written_count", O_CREAT | O_EXCL, 0666, 1);
 
 thread_safe("PTHREAD_CREATE", pthread_create(&father, NULL, father_thread, NULL));
 thread_safe("PTHREAD_CREATE", pthread_create(&son, NULL, son_thread, NULL));
 thread_safe("PTHREAD_JOIN", pthread_join(father, NULL));
 thread_safe("PTHREAD_JOIN", pthread_join(son, NULL));


}

void* father_thread(void* arg) {

 int element;
 
 sem_wait(increase_written_count);
 for(N_written = 0; N_written<N; N_written++) {
	sem_post(increase_written_count);
 	sem_wait(acquire_write_right); 
	file_safe("WRITE", write(gfd, &N_written, sizeof(int)));
	lseek(gfd, 0, SEEK_SET);
	file_safe("READ", read(gfd, &element, sizeof(int)));
	lseek(gfd, 0, SEEK_SET);
	printf("FATHER: NUMBER - %d\n",element);
	sem_post(acquire_write_right);
	
	sem_wait(increase_written_count);
 }

 sem_post(increase_written_count);

}

void* son_thread(void* arg) {
 
 int element;

 sem_wait(increase_written_count);
 for(N_written = 0; N_written<N; N_written++) {
	sem_post(increase_written_count);
	sem_wait(acquire_write_right); 
	file_safe("WRITE", write(gfd, &N_written, sizeof(int)));
	lseek(gfd, 0, SEEK_SET);
	file_safe("READ", read(gfd, &element, sizeof(int)));
	lseek(gfd, 0, SEEK_SET);
	printf("SON: NUMBER - %d\n",element);
	sem_post(acquire_write_right);
	
	sem_wait(increase_written_count);
 }
	
 sem_post(increase_written_count);

}

void thread_safe(char* s_err, int n_err) {
 
 if(n_err != 0) {
 	fprintf(stderr, "%s: %d\n",s_err, n_err);
	exit(EXIT_FAILURE);
 }
}

int file_safe(char* s_err, int n_err) {
 
  if(n_err == -1) {
 	perror(s_err);
	exit(EXIT_FAILURE);
 }

 return n_err;

}
