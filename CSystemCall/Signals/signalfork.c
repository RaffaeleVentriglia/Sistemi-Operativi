#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

static void signal_handler(int);

int main() {

pid_t childpid = 0;
signal(SIGINT, signal_handler);

if((childpid = fork()) < 0) {
	perror("FORK");
	exit(EXIT_FAILURE);
} else if (childpid > 0) wait(NULL); //the father will attend forever 
    else if(childpid == 0) {
		sleep(5); //let the father wait 
		printf("SON NOW:\n");
		while(1) 
			sleep(1);
	}
}

static void signal_handler(int signo) {

if(signo == SIGINT) printf("SIGNAL RECEIVED: %d\n",signo);

}
