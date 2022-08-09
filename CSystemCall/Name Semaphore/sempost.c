#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main(int argc, char* argv[]) {

 sem_t* generic_sem;
 
 if(argc!=2) {
	fprintf(stderr, "USAGE: ./sempost <semaphore name>");
	exit(EXIT_FAILURE);
 }

 generic_sem = sem_open(argv[1],0);
 sem_post(generic_sem);
 return(0);

}
