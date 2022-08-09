#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_mediana(void*);
void thread_safe(char*, int);

double mediana = 0;
int** matrix;
int N;
sem_t* sem_access;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) {

 srand(time(NULL));

 if(argc!=2) {
	printf("Usage: ./filename <N>\n");
	exit(EXIT_FAILURE);

 } else N = atoi(argv[1]);

 pthread_t tids[N];
 int t_ids[N];

 //dinamic matric
 matrix = malloc(N*sizeof(int*));
 for(int i=0;i<N; i++) {
	matrix[i] = malloc(N*sizeof(int));
 }



  //random filling 
 for(int i=0;i<N;i++) {
	for(int j=0;j<N;j++) {
		matrix[i][j] = rand()%10+1;
	}
 }

 printf("Matrice:\n");
  for(int i=0;i<N;i++) {
	for(int j=0;j<N;j++) {
		printf("%d ", matrix[i][j]);
	}
	puts("");
 }

 if((sem_access = sem_open("sem_access", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "SEM_ACCESS: ERROR\n");
	exit(EXIT_FAILURE);
 }
 


 for(int i=0; i<N; i++) {
	t_ids[i] = i;
	thread_safe("pthread_create", pthread_create(&tids[i], NULL, thread_mediana, &t_ids[i]));
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread_join", pthread_join(tids[i], NULL));
 }

 mediana = mediana/N;
 printf("Valore mediano massimo della matrice: %.1f\n", ceil(mediana));

 sem_close(sem_access);
 sem_unlink("sem_access");
 
 for(int i=0;i<N;i++) {
	free(matrix[i]);
 } 


 return(0);
}

void* thread_mediana(void* arg) {

 int row = *(int*)arg;
 double sum = 0;

 printf("T%d\n",row);
 
 for(int i=0; i<N; i++) {
	sum += matrix[row][i];
 }

 sum = sum/N;
 
 sem_wait(sem_access);
 pthread_mutex_lock(&mutex);
 mediana+=sum;
 pthread_mutex_unlock(&mutex);
 sem_post(sem_access);
 pthread_exit(NULL);
 

}

void thread_safe(char* serr, int nerr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n",serr,nerr);
	exit(EXIT_FAILURE);
 }

}
