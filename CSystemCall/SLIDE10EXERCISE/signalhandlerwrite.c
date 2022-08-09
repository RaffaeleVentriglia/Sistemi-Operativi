#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static void signal_handler(int);

int main() {

 //Initializing phase
 struct sigaction act, oact;
 sigset_t set;

 //defining signal mask 
 sigfillset(&set);
 
 //defining sigaction struct & calling it 
 act.sa_handler = &signal_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 sigaction(SIGINT, &act, &oact);

 //LOOP
 while(1);

}

static void signal_handler(int signo) {

 if(write(STDOUT_FILENO, "I don't wanna die!\n", 20) != 20) {
	perror("WRITE");
 	exit(EXIT_FAILURE);
 }

} 
