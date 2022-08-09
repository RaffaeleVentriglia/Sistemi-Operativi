#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void signal_handler(int signo);

int main() {

//initializing
 struct sigaction act;
 sigset_t set;
 char buffer[64];

 //initializing sigset_t set for blocking signals while in the handler 
 sigfillset(&set);

 //defining now sigaction structure and calling sigaction function to install the new handler
 act.sa_handler = &signal_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 sigaction(SIGINT, &act, NULL);
 
 //now call a system call 
 if(read(STDIN_FILENO, &buffer, sizeof(buffer)) < 0) {
	perror("READ");
	exit(EXIT_SUCCESS);
 }

 printf("READ WITH FUNCTION READ: %s\n",buffer);

}

static void signal_handler(int signo) {

 printf("CAUGHT SIGINT\n");

}
