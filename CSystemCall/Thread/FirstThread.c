#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* Thread_Fun(void*);

int main() {

 char* str = "Thread N.1 Created.\n";
 pthread_t tid; //thread id of the new thread

 pthread_create(&tid, NULL, Thread_Fun, (void*)str);
 sleep(1);
 printf("Main thread.\n");


}

void* Thread_Fun(void* arg) {

 printf("%s",(char*)arg);

}
