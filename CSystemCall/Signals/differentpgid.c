#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

//purpose of this program is to demonstrate that if we fork a process, 
//and change the process group id of a son process, he will not receive the same
//signals as the father. 

static void signal_handler(int);

int main() {

 //Initialization
 int counter = 0;
 pid_t childpid;
 struct sigaction act, oact;
 sigset_t set;

 //defining the mask for signals to block while in the handler
 sigfillset(&set);

 //defining sigaction struct and calling the function to install the handler
 act.sa_handler = &signal_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 sigaction(SIGINT, &act, &oact);

 //now forking
 if((childpid = fork()) < 0) {
	perror("FORK");
	exit(EXIT_FAILURE);
 }

 //now, both of the signals will do something on the catch of SIGINT
 //if CTRL+C will be pressed until 10 seconds, we can interrupt both 
 //for 10 times.
 printf("BOTH SON/FATHER CANT BE INTERRUPTED WITH SIGINT\n");
 while(counter < 10) {
	sleep(10);
	counter++;
 }

 //now changing the gpid of the son with his PID:
 if(childpid == 0) 
 setpgid(0, getpid());

 //resetting the counter 
 counter = 0;
 
 printf("NOW ONLY THE FATHER WILL MANAGE SIGINT\n");
 //now only the father will manage sigint.
 sleep(5);

 //restoring the old pgid to the son: now they'll manage the handler toghether
 if(childpid > 0) //if the father
 setpgid(childpid,getpid()); //restore old pgid : set the pgid (2nd arg) of the pid specified (1st arg)

 //resetting the counter
 counter = 0;
 
 printf("BOTH SON/FATHER CANT BE INTERRUPTED WITH SIGINT AGAIN\n");
 printf("ID %d - PGID %d\n", getpid(), getpgid(0));
 //now they manage it both again 
 while(counter < 10) {
	sleep(10);
	counter++;
 }

 //restoring the old handler now; they can now be interrupted
 sigaction(SIGINT, &oact, 0);
 
 printf("THEY CAN NOW BOTH BE INTERRUPTED.\n");
 //wait for the signals. 
 while(1);

}

static void signal_handler(int signo) {

 printf("SIGINT CATCHED - ID %d\n",getpid()); 

}
