#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int scounter = 0;
pid_t fatherID = 0;

//In this program we'll demonstrate how changing a SON GPID will avoid him
//receiving signals from his father. Because signal can be sent only to the process with the same 
//group id of the main control process (in this case, our father)

static void signal_handler(int);



int main() {

//Initialization:
 pid_t childpid = 0;
 struct sigaction act,oact;
 sigset_t set;

 //assigning father id
 fatherID = getpid();

 //defining the sigaction struct and the signal to be blocked while in handler
 sigfillset(&set);
 act.sa_handler = &signal_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 //end of sigaction defining

 //calling sigaction
 sigaction(SIGINT, &act, &oact);

  //signal handling before changing gpid 
 printf("SIGNAL HANDLING BEFORE CHANGING GPID.\n");
 printf("YOU CAN INTERRUPT 3 TIMES BEFORE THE CHANGE.\n");

 //forking process
 if((childpid = fork()) < 0) {
	perror("FORK");
 	exit(EXIT_FAILURE);
 }
 
 while(scounter < 3);

 //changing GPID of the son:
 if(getpid() == fatherID)
 printf("NOW CHANGING PGID OF SON...\n");
 else sleep(1);

 if(childpid == 0) setpgid(0,getpid());
 
 printf("PID: %d - GPID: %d\n", getpid(), getpgid(0));

 //loop to check
 while(1);
 
}


static void signal_handler(int signo)  {

 printf("CAN'T BE BLOCKED FROM CTRL+C: PROCESS ID: %d - GPID: %d\n",getpid(), getpgid(0));
 scounter++;

}
