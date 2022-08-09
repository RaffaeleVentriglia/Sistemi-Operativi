#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void* calculate_diagonal(void*);

int ***matrix;
int minimum[3][1];


int main(int argc, char* argv[]) {

 srand(time(NULL));

 int N;
 
 if(argc!=2) {
	printf("Usage: ./filename <N>\n");
	exit(EXIT_FAILURE);

 } else N = atoi(argv[1]); 
 
 pthread_t tids[N];
 int t_ids[N];
 
 //allocation phase 
 matrix = malloc(N*sizeof(int**));

 //x: external block of matrix, i row block of matrix, j columns of matrix

 for(int x=0; x<N; x++) {
	matrix[x] = malloc(3*sizeof(int*));	
 }

 for(int x=0; x<N; x++) {
	for(int i=0; i<3; i++) {
		matrix[x][i] = malloc(3*sizeof(int));
	}
 }
 
 
 //random filling
 for(int x=0; x<N; x++) {
 	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			matrix[x][i][j] = rand()% 100+0;
		}
 	}
 } 

 for(int x=0; x<N; x++) {
 	for(int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			printf("%d ", matrix[x][i][j]);
		}
		puts("");
 	}
	puts("");
 }	

 //end allocation phase 

 for(int i=0; i<N; i++) {
	t_ids[i] = i;
 	if(pthread_create(&tids[i], NULL, calculate_diagonal, &t_ids[i]) != 0) {
		fprintf(stderr, "pthread_create");
		exit(EXIT_FAILURE);
	}
 }

 for(int i=0; i<N; i++) {
	if(pthread_join(tids[i], NULL) != 0) {
		fprintf(stderr, "pthread_join");
		exit(EXIT_FAILURE);
	}
 }


}

void* calculate_diagonal(void* arg) {

 int matrix_block = *(int*)arg;
 int min = 100;
 

 //left diagonal
 for(int i=0; i<3; i++) {
 	if(matrix[matrix_block][i][i] < min) {
		min = matrix[matrix_block][i][i];
	}
 }
 
 //right diagonal
 for(int i=0; i<3; i++) {
	if(matrix[matrix_block][i][2-i] < min) {
		min = matrix[matrix_block][i][2-i];
	}

 }

 printf("Matrix %d diagonals minimum: %d\n",matrix_block, min);

 pthread_exit(NULL);

}
