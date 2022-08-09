#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 10

void* matrix_search(void*);
void matrix_init(void);
void matrix_print(void);
void thread_safe(char*, int);

struct info {
 int my_id;
 char to_search;
};

struct {
 int row;
 int column;
 int thread_id;
} coordinate;

typedef struct info s_info;

pthread_once_t flag = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t tids[N];
char array[N][N];
int found = 0;

int main(int argc, char* argv[]) {
 srand(time(NULL));

 char x = *(char*)argv[1];
 s_info thread_info[N];

 for(int i=0; i<N; i++) {
	thread_info[i].my_id = i;
	thread_info[i].to_search = x;
	thread_safe("pthread create", pthread_create(&tids[i], NULL, matrix_search, &thread_info[i]));
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread join", pthread_join(tids[i], NULL));
 }

 printf("Matrix:\n");
 matrix_print();
 printf("\n");
 printf("Char %c found at:\nRow - %d\nColumn - %d\nThread ID: %d\n",x, coordinate.row, coordinate.column, coordinate.thread_id);

 pthread_mutex_destroy(&mutex);
 return(0);
 
}

void thread_safe(char* s_err, int n_err) {

 if(n_err != 0) {
	fprintf(stderr, "%s : %d", s_err, n_err);
	exit(EXIT_FAILURE);
 }

}

void* matrix_search(void* arg) {

 s_info my_struct = *(s_info*)arg;
 int myRow = my_struct.my_id;
 char toSearch = my_struct.to_search;

 pthread_once(&flag, matrix_init);

 for(int column = 0; column < N; column++) {
	if(array[myRow][column] == toSearch) {
		pthread_mutex_lock(&mutex);
		if(found == 1) {
			printf("Char already found.\n");
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		} else {
			found = 1;
			pthread_mutex_unlock(&mutex);
			for(int i=0; i<N; i++) {
				if(pthread_equal(pthread_self(), tids[i]) == 0) { //if not equal (pthread equal return 0 if not equal, != if equal)
					pthread_cancel(tids[i]);
				}
			}

			coordinate.row = myRow;
			coordinate.column = column;
			coordinate.thread_id = myRow;
		  }
 	}
 }
 
 pthread_exit(NULL);

}

void matrix_init() {

 for(int i=0; i<N; i++) {	
	for(int j=0; j<N; j++) {
		array[i][j] = (char)(rand()%(122 + 1 - 97) + 97);
 	}
 } 

}

void matrix_print(void) {

 for(int i=0; i<N; i++) {
	for(int j=0; j<N; j++) {
		printf("%c ", array[i][j]);
	}
	printf("\n");
 }

}
