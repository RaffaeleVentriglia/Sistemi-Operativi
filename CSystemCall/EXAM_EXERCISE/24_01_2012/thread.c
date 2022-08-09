#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <pthread.h>


void* thread_reader(void*);
void* thread_writer(void*);
void thread_safe(char*, int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int var_to_read = 4;
int is_read = 1;
int is_full = 0;
int row = 0;
int column = 0;
int matrix[5][5];

int main() {
 
 srand(time(NULL));
 int N = 7;
 pthread_t reader_pids[7];
 pthread_t writerpid;

 thread_safe("writer pid", pthread_create(&writerpid, NULL, thread_writer, NULL));
 
 for(int i=0; i<7; i++) {
 	thread_safe("reader pid", pthread_create(&reader_pids[i], NULL, thread_reader, NULL));
 }
 
 for(int i=0; i<7; i++) {
	thread_safe("reader join", pthread_join(reader_pids[i], NULL));
 }
 
 thread_safe("reader join", pthread_join(writerpid, NULL));

 for(int i=0;i<5;i++) {
	for(int j=0; j<5; j++) {
		printf("%d ", matrix[i][j]);
	}
	puts("");
 }


}

void* thread_writer(void* arg) {

 while(is_full ==0) {
	pthread_mutex_lock(&mutex);
	var_to_read = rand()%100+1;
	printf("%d\n",var_to_read);
	sleep(1);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	sleep(3);
 }

}


void* thread_reader(void* arg) {
 printf(":");
 int to_read;
 while(is_full == 0) {
	pthread_mutex_lock(&mutex);
	while(is_read == 0) {
		pthread_cond_wait(&cond, &mutex);
	}

 	if(is_full == 1) break;
	
	is_read = 0;
	to_read = var_to_read;	

	//matrix safe check
 	if(row == 4 && column == 4) {
		is_full = 1;
		for(int i=0;i<7;i++) {
			pthread_cond_signal(&cond);	
		}
		break;
	}

	if(column == 4) {
		row++;
		column=0;
	}

	matrix[row][column] = to_read;
	
	column++;
	pthread_mutex_unlock(&mutex);

 }
 
}

void thread_safe(char* serr, int nerr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n", serr, nerr);
	exit(EXIT_FAILURE);

 }


}
