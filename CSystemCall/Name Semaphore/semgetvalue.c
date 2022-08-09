#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main(int argc, char* argv[]) {

 int value;
 sem_t* sem;
 
 if(argc!=2) {
	fprintf(stderr, "USAGE: ./semgetvalue <semaphore name>");
	exit(EXIT_FAILURE);
 }

 sem = sem_open(argv[1], 0);
 sem_getvalue(sem, &value);
 printf("value of %s is: %d\n", argv[1], value);
 return(0); 

}
