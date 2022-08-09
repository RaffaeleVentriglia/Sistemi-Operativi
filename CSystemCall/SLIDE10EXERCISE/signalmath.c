#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int a = 0;
int b = 0;

static void multiple_handler(int);

int main(int argc, char* argv[]) {
 
 //Init phase
 struct sigaction act;
 sigset_t set;

 //defining the signal list to block while in handler
 sigfillset(&set);

 //assign the input argument to global variables
 a = atoi(argv[1]);
 b = atoi(argv[2]);

 //defining sigaction and calling it to manage our signals
 act.sa_handler = &multiple_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 sigaction(SIGUSR1, &act, 0);
 sigaction(SIGUSR2, &act, 0);
 sigaction(SIGINT, &act, 0);

 //LOOP to send SIGUSR1 and SIGUSR2
 while(1) {
 	sleep(3);
 	kill(getpid(), SIGUSR1);
 	sleep(3);
 	kill(getpid(), SIGUSR2);
	sleep(3);
 }


}

static void multiple_handler(int signo) {

 if(signo == SIGUSR1) printf("difference betwenn %d and %d is: %d\n", a, b, a-b);
 else if (signo == SIGUSR2) printf("sum betwenn %d and %d is: %d\n", a, b, a+b);
 else if (signo == SIGINT) {
 	printf("RECEIVED SIGINT, TERMINATION..\n");
 	exit(EXIT_SUCCESS);
 }
}
