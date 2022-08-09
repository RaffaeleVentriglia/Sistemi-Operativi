#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int main(int argc, char* argv[]) {

 sem_t* generic_sem;
 
 if(argc!=2) {
	fprintf(stderr, "USAGE: ./semwait <semaphore name>");
	exit(EXIT_FAILURE);
 }

 generic_sem = sem_open(argv[1], 0);
 printf("%d going to acquire the semaphore..\n", getpid());
 sem_wait(generic_sem);
 printf("semaphore acquired. going to sleep... (no signal)\n");
 pause();
 return(0);

}
