#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

static void signal_handler(int);

int main() {

 pid_t childpid1, childpid2;
 
 signal(SIGINT, signal_handler);

 if((childpid1 = fork()) == 0) {
	while(1) {
		printf("PROCESS 1 ALIVE\n");
		sleep(1);
	}
 } else if (childpid1 > 0) {
   	if((childpid2 = fork()) == 0) {
		while(1) {
			printf("PROCESS 2 ALIVE\n");
			sleep(1);		
		}
 
   	}
   }


 //father now: suspend first son after 2 seconds;
 sleep(2);
 kill(childpid1, SIGSTOP);

 //wait another 2 seconds and reactivate the first son again:
 sleep(2);
 kill(childpid1, SIGCONT);

 //sleep 2 more seconds and kill both.
 sleep(2);
 kill(childpid1, SIGINT);
 kill(childpid2, SIGINT);

}

static void signal_handler(int signo) {

 if(signo == SIGINT) {
 printf("PROCESS %d TERMINATED BY HIS FATHER %d\n",getpid(), getppid());
 exit(0); 
}

}

