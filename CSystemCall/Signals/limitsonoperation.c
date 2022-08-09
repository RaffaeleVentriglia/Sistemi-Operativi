#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void signal_handler(int);

int main (int argc, char* argv[]) {

pid_t childpid; 
int delay = atoi(argv[1]);

 if(signal(SIGCHLD, signal_handler) == SIG_ERR) {
	perror("SIGNAL");
	exit(EXIT_FAILURE);
 }

 if((childpid = fork()) > 0) {
	printf("FATHER %d WAITING %d SECONDS BEFORE KILLING MY SON.\n", getpid(), delay);
	sleep(delay);
 } else if (childpid == 0) {
	execvp(argv[2], argv+2);
   }
 
 //father took back the control because exec make a child terminate
 printf("CHILD %d EXCEED TIME. KILLING IT..\n", childpid);
 kill(childpid, SIGINT);
 
 return(0);

} 

static void signal_handler(int signo) {

int childstatus;
pid_t childpid;

childpid = wait(&childstatus); //returning into childpid the terminated son process with status childstatus
printf("CHILD %d TERMINATED WITHIN THE RIGHT SECONDS.\n",childpid);
exit(EXIT_SUCCESS);
 
}




