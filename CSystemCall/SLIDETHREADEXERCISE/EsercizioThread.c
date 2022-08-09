#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#define M 3
#define N 4
#define P 5

void* compute_matrix(void*);
void matrix_init(void);
void matrix_print(int**,int,int);
int **A, **B, **C;

int main() {

 pthread_t tids[M];
 int t_id[M];
 int err;

 srand(time(NULL));
 matrix_init();
 

 for(int i = 0; i<M; i++) {
	t_id[i] = i;	
	//to each trhead we'll pass his referement number to indicate on which row he should work.
	if((err = pthread_create(&tids[i], NULL, compute_matrix, &t_id[i])) != 0) {
		fprintf(stderr, "PTHREAD_CREATE: %d", err);
		exit(EXIT_FAILURE);
	} else printf("THREAD %d CREATED: OK\n",t_id[i]);
 }
 
 for(int i = 0; i<M; i++) {
	if((err = pthread_join(tids[i], NULL)) != 0) {
		fprintf(stderr, "PTHREAD_JOIN: %d", err);
	} else printf("WAITED FOR THREAD %d: OK\n",t_id[i]);
 }

 puts("");
 printf("Matrix A:\n");
 matrix_print(A,M,N);
 puts("");
 printf("Matrix B:\n");
 matrix_print(B,N,P);
 puts("");
 printf("Matrix C (product A*B):\n");
 matrix_print(C,M,P);
 puts("");

}

void matrix_init(void) {
 int i,j;
 A = malloc(M*sizeof(int*)); //allocation for the rows of Matrix A
 B = malloc(N*sizeof(int*)); //allocation for the rows of Matrix B
 C = malloc(M*sizeof(int*)); //allocation for the rows of matrix C

 for(i = 0; i<M; i++) {
	A[i] = malloc(N*sizeof(int)); //allocation for the columns of matrix A
 }

 for(i = 0; i<N; i++) {
	B[i] = malloc(P*sizeof(int)); //allocation for the columns of matrix B
 }

 for(i= 0; i<M; i++) {
	C[i] = malloc(N*sizeof(int)); //allocation for the columsn of matrix C;
 }

 //random filling of A
 for(i = 0; i<M; i++) {
	for(j = 0; j<N; j++) {
		A[i][j] = rand() % 10;
 	}
 }

 //random filling of A
 for(i = 0; i<N; i++) {
	for(j = 0; j<P; j++) {
		B[i][j] = rand() % 10;
 	}
 }
 
}

void matrix_print(int** Matrix, int Row, int Column) {
 int i,j;
 
 for(i=0; i<Row; i++) {
	for(j=0; j<Column; j++) {
		printf("%d ",Matrix[i][j]);
 	}	
	printf("\n");
 }
}

void* compute_matrix(void* arg) {
 
 int Arow = *(int*)arg;
 int Acolumn;
 int Brow;
 int Bcolumn;
 int ith_no = 0;

 //iterating through the row of B 
 for(Bcolumn = 0; Bcolumn < P; Bcolumn++) {
	//itherathing trought the column of A
	for(Acolumn = 0; Acolumn < N; Acolumn++) {
		//the row of B is equal to the column of A
		Brow = Acolumn;
		//do the product
		ith_no += A[Arow][Acolumn]*B[Brow][Bcolumn];
	}
	

 	C[Arow][Bcolumn] = ith_no;
	Acolumn = 0;	
 }
 

}
