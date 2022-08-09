#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 10

void* synch_thread(void*);
void print_vector(int*);

pthread_mutex_t Mutex;
int indice_array = 0;
int array[N];


int main() {

 pthread_t tids[N];
 int t_id[N];
 int err;

 //initializing the mutex
 pthread_mutex_init(&Mutex, NULL); 

 for(int i=0; i<N; i++) {
	t_id[i] = i;
 	if((err = pthread_create(&tids[i], NULL, synch_thread, &t_id[i])) != 0) {
		fprintf(stderr, "THREAD NOT CREATED: %d", err);
		exit(EXIT_FAILURE);
 	}
 
 }

 for(int i=0; i<N; i++) {
	if(indice_array >= N) break;
	pthread_mutex_lock(&Mutex);
	array[indice_array] = i;
	indice_array++;
	pthread_mutex_unlock(&Mutex);
	printf("MAIN: value of array[]: %d - Index: %d\n", array[indice_array], indice_array);	

 }
 
 //wait the last thread
 for(int i=0; i<N; i++) {
	pthread_join(tids[i], NULL);
 }
 
 print_vector(array);

 return(0);

}

void* synch_thread(void* arg) {

 printf("Thread id: %d\n", *(int*)arg);
 puts("");

 for(int i=0; i<N; i++) {
	if(indice_array >=N) break;
	pthread_mutex_lock(&Mutex);
	array[indice_array] = i;
	indice_array++;
	pthread_mutex_unlock(&Mutex);
	printf("Thread %d: value of array[]: %d - Index: %d\n",*(int*)arg, array[indice_array], indice_array);	
	
 }

 pthread_exit(NULL);

}

void print_vector(int a[]) {

 printf("Vector values: ");
 for(int i = 0; i<N; i++) {
	printf("%d ", array[i]);
 }

}





