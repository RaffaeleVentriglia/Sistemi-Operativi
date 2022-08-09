#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main(int argc, char* argv[]) {

 if(argc!=2) {
	fprintf(stderr, "USAGE: ./semunlink <semaphorename>");
	exit(EXIT_FAILURE);
 }

 sem_unlink(argv[1]);

}
