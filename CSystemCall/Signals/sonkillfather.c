#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

static void signal_handler(int);

int main() {

pid_t childpid;
 
 if(signal(SIGUSR1, signal_handler) == SIG_ERR) {
	perror("CANT DEFINE NEW HANDLER");
	exit(EXIT_FAILURE);
 } //defyining new signal handler 

 //now forking 
 if((childpid = fork()) < 0) {
	perror("FORK");
	exit(EXIT_FAILURE);
 } else if (childpid == 0) { //if son
	printf("SON ID: %d\n",getpid());
	printf("GOING TO SLEEP FOR 3 SECONDS,THEN KILL MY FATHER %d\n",getppid());
	for(int i = 0; i <3; i++) {
		sleep(1);
		printf(".\n");
	}

	kill(getppid(),SIGUSR1);
	printf("SON DONE, DYING\n");
	_exit(EXIT_SUCCESS);

  } else if (childpid > 0) { //if father
	printf("FATHER ID: %d\n",getpid());
	printf("DOING MY THINGS\n");
	for(int i = 0; i<6; i++) {
		sleep(1);
		printf("*\n");
	}
	
	printf("OPERATION COMPLETED, DYING.\n");
	exit(EXIT_SUCCESS);
   }
}

static void signal_handler(int signo) {

if(signo == SIGUSR1) 
	printf("\nINTERRUPT RECEIVED %d\n",signo);
 return; //optional: with or without, the handler will return anyway
}
