#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int commandnum;
pid_t gchildpidA;
pid_t gchildpidB;


static void odd_handler(int);
static void even_handler(int);
static void validation_check(int,char*[]);

int main(int argc, char* argv[]) {

 //check if the argument is valid, and if there is at least 1 argument
 validation_check(argc,argv);

 //initialization
 pid_t childpid = 0;
 int positivenum = atoi(argv[1]);
 struct sigaction act;
 sigset_t set;

 //assigning the positive num to a global variable 
 commandnum = positivenum;

 //defining the signals to block during the execution of the handler
 sigfillset(&set);
 

 //before forking, we need to define the handler through the sigaction.
 //so we check in what situation we are, and choose which handler install
 if(positivenum % 2 == 0) {
 	act.sa_handler = &odd_handler;
	act.sa_mask = set;
	act.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
 } else { //the number is even 
	act.sa_handler = &even_handler;
	act.sa_mask = set; 
	act.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
   }

 //now creating two son: 
 //creating first son A:
 if((childpid = fork()) < 0) {
	perror("FORK");
	exit(EXIT_FAILURE);
 } else if(childpid > 0) {
	//definin global gchildpidA for future use of father/son process
	gchildpidA = childpid;
	//creating second son B
	if((childpid = fork()) < 0) {
		perror("FORK");
		exit(EXIT_FAILURE);
	} else if(childpid > 0) { //if the father, update global gpidchildB for the future use of THE FATHER ONLY
		gchildpidB = childpid;
	  } 
 }

 //both of sons will have childpid = 0. so we'll use that to check is a process 
 //is a son or not.
 //the sons must wait until the reception of SIGUSR1 or SIGUSR2
 if(childpid == 0) pause();
 
 //this part of code will be executed exclusively from the father, because A & B are stopped.
 //now, if the number is odd we need to send SIGUSR1 to both the process A & B.
 if(positivenum % 2 == 0) {
	//changing process group id of B for letting him execute a command 
 	//at the reception of SIGUSR1 different from A.
	setpgid(gchildpidB, gchildpidB);
 	kill(gchildpidA, SIGUSR1);
 	kill(gchildpidB, SIGUSR1);
 } else { 
	//if the number is even we only need to send SIGUSR2 to B.
	//Process B will then send SIGUSR1 to A.
	kill(gchildpidB, SIGUSR2);
   }

 //we also need to make sure the process will not be zombies; so cast a wait on the children to 
 //make that not happen.
 while(wait(NULL) != -1);
 printf("FATHER ID %d - TERMINATED.\n",getpid());
 return(0);
}


//handler to manage in case the number passed by terminal is odd
static void odd_handler(int signo) {
  
 if (signo == SIGUSR1) {
	//if the PGID of the child process is equal to the PGID of father process
	//then is the childpidA son.
	if(getpgid(getpid()) == getpgid(getppid())) {
	printf("SON A ID %d - CUBE OF %d IS: %d\n",getpid(), commandnum,(int)pow(commandnum,3));	
	_exit(EXIT_SUCCESS); 
	} else { //else is process B
		sleep(1);
		printf("SON B ID: %d - GOODBYE!\n",getpid());
		_exit(EXIT_SUCCESS);
 	} //end else
    } //end external if
} //end function


//handler to manage in case the number passed by terminal is even
static void even_handler(int signo) {

 if(signo == SIGUSR2) { //IF PROCESS B 
	printf("SON B ID: %d - THE RECIPROCAL OF %d IS: 1/%d = %.3f\n",getpid(), commandnum, commandnum, 1/(float)commandnum);
	printf("NOW SLEEPING FOR %d SECONDS..\n",commandnum*2);
	//sleep for the amount of command num * 2
	sleep(commandnum*2);
	//send the signal to A
	kill(gchildpidA, SIGUSR1);
	_exit(EXIT_SUCCESS); 
} else if (signo == SIGUSR1) { //IF PROCESS A
	 printf("SON A ID: %d - TERMINATED BY SIGUSR1 BY SON B..\n",getpid());
 	_exit(EXIT_SUCCESS);
  } //end else
} //end function

//function to validate the input given
static void validation_check(int argc, char* argv[]) {

 if(argc!=2) {
	printf("TOO FEW/MANY ARGUMENTS. EXITING..\n");
	exit(EXIT_FAILURE);
 } else {
	if(atoi(argv[1]) <=0) {
		printf("ARGUMENT INVALID. MUST BE A POSITIVE INTEGER.\n");
		exit(EXIT_FAILURE);
	}//end if
   } //end else
} //end function



