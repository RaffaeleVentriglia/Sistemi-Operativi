#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//This program demonstrate that every thread has is own stack when declaring variables in their local scope.
//All the variables inherited from the main tread instead, are accessible from all the thread.  (critical section)

void* pthread_fun(void*);
int globalvar = 1;

int main() {

 pthread_t tid1,tid2;
 int t_id[2] = {0};
 

 pthread_create(&tid1, NULL, pthread_fun, &t_id[0]);
 t_id[1]++;
 pthread_create(&tid2, NULL, pthread_fun, &t_id[1]);

 pthread_join(tid1,NULL);
 pthread_join(tid2,NULL);

}

void* pthread_fun(void* arg) {

int tt_id = *(int*)arg + 1;
int localvar = 1;
printf("t_id = %d\n",tt_id);
 for(int i = 0; i<50; i++) {
	if(tt_id == 1) {
		printf("T1: local %d - global %d\n",localvar,globalvar);	
		globalvar++;
		sleep(1);
	} else if (tt_id == 2) {
		printf("T2: local %d - global %d\n",localvar,globalvar);
		localvar++;
		globalvar++;
		sleep(1);
	  } 
 }


}


