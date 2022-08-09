#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_product(void*);
void thread_safe(char*, int);

int** matrix;
int max_product = 0;
int N;

sem_t* sem_access;

int main(int argc, char* argv[]) {

 srand(time(NULL));
 
 if(argc!=2) {
	printf("Usage: ./filename <N>\n");
	exit(EXIT_FAILURE);
 } else N = atoi(argv[1]);

 pthread_t tids[N];
 int t_ids[N];

 matrix = malloc(N*sizeof(int*));
 for(int i=0; i<N; i++) {
	matrix[i] = malloc(N*sizeof(int));
 }

 for(int i=0; i<N; i++) {
	for(int j=0; j<N; j++) {
		matrix[i][j] = rand() % 255+0;
 	}
 }
 
  printf("Matrix:\n");
  for(int i=0; i<N; i++) {
	for(int j=0; j<N; j++) {
		printf("%d ", matrix[i][j]);
 	}
 	puts("");
 }

 if((sem_access = sem_open("sem_access", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "sem open");
	exit(EXIT_FAILURE);
 }

 for(int i=0; i<N; i++) {
	t_ids[i] = i;
	thread_safe("pthread_create", pthread_create(&tids[i], NULL, thread_product, &t_ids[i]));
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread_join", pthread_join(tids[i], NULL));
 }

 printf("Maximum of row product of the matrix: %d\n",max_product);
 sem_close(sem_access);
 sem_unlink("sem_access");

 for(int i=0; i<N; i++) {
	free(matrix[i]);
 }


}

void* thread_product(void* arg) {

 int row = *(int*)arg;
 int product_sum = 1;
 for(int i=0; i<N; i++) {
	product_sum *= matrix[row][i];
 }  

 sem_wait(sem_access);
 if(product_sum > max_product) {
	max_product = product_sum;
 }
 sem_post(sem_access);
 pthread_exit(NULL);

}

void thread_safe(char* serr, int nerr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n", serr, nerr);
	exit(EXIT_FAILURE);
 }

}

