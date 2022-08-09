#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#define MAX_CHAR_SEM 15

int main(int argc, char* argv[]) {

 int c, value = 1; //int c is a value to represent a char (in c comparison between int and char are promoted to int - int)
 int open_flag = O_CREAT;
 char* name = malloc(MAX_CHAR_SEM*sizeof(char));
 sem_t* sem; 

 while((c = getopt(argc, argv, "en:v:")) != -1) {
	switch(c) {
		case 'e':
		open_flag |= O_EXCL;			
		break;

		case 'n':
		strcpy(name, optarg);
		break;
		
		case 'v':
		value = atoi(optarg);
		break;

		default:
		abort();
	}
 }

 sem = sem_open(name, open_flag, 0755, value);
 sem_close(sem);
 return(0);

}
