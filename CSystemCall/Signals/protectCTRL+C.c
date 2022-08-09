#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//program that "protect" portion of code from CTRL + C, 
//restoring, after a number of seconds, to the old handler.

static void signal_handler(int);

typedef void (*prev_handler)(int); //prev_handler is now an alias to specify the old handler.

int main () {

 //defyining the old handler
 prev_handler old_handler;
 
 //ignoring the signal 
 if((old_handler = signal(SIGINT, SIG_IGN)) == SIG_ERR) {
	perror("SIGNAL");
	exit(EXIT_FAILURE);
 }
 
 printf("Can't be interrupted now!\n");
 sleep(5);
 
 //restoring the signal
 if(signal(SIGINT, old_handler) == SIG_ERR) {
	perror("SIGNAL");
	exit(EXIT_FAILURE);
 }

 printf("\nCan be interrupted now!\n");
 sleep(5);

 return(0);
}

static void signal_handler(int signo) {
 
 printf("CTRL+C DISABLED.\n");
 
}
