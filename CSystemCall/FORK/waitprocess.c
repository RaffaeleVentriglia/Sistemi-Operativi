#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

 pid_t fatherID;
 pid_t childpid = 0;
 pid_t waitchild = 0;
 int pnum;

 //rooting the parent id
 fatherID = getpid();

 //if argument passed by line are too much or too few, exit
 if(argc!=2) { 
 printf("TOO MUCH/TOO FEW ARGUMENTS\n");
 exit(EXIT_FAILURE);
 }
 
 //translate the string into a number
 pnum = atoi(argv[1]);
 
 //for the number the user submit to create pnum processs, do
 for(int i = 1; i<pnum; i++) {
	//create pnum process:
	//we only want to create ONE process each: so break if fork is successful.
 	if((childpid = fork()) < 0) {
	 perror("FORK");
	 exit(EXIT_FAILURE);
	 }//endif
	   //why we put "else if childpid > 0?" because we need to think that when the fork is
	   //executed, two process are active now, so both of them will go in the else statement
	   //if the fork is successful, and break their cycle, both of them. so only two process
	   //will be printed. WE NEED to specify that only the father need to break, not the son.
	  else if (childpid > 0) break;
 } //end for

 //now, when the process break from a successful fork, we want to make it wait
 //until is son, grandson etc are terminated, so we make every process parent who enter this
 //cycle wait, except for the last process created: it will be te first to be printed.

 while(childpid != (waitchild = wait(NULL))) { //check every instant if a process is terminated.
	//waitchild is the variables that contains the PID of the terminated process, by wait.
	//-1 if no process are active.
	if(waitchild < 0) {
		break;
 	} //end if
 } //end while

 //the process will be executed from the last to the first (the root parent)
 printf("I'm the process ID: %d, with father ID: %d\n",getpid(), getppid());

 //now, if a SON/GRANDSON process is terminating is cycle, _exit without clean the buffer, 
 //otherwise, exit with cleanup operations.
 if(getpid() == fatherID)
 exit(EXIT_SUCCESS);
 else _exit(EXIT_SUCCESS);

}
