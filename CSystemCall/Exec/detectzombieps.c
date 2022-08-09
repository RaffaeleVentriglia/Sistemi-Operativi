#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//program to create a zombie process and then execute ps to verify that.

int main() {

 pid_t childpid;
 pid_t wpid;
 
 //create two process son: we'll have 3 process in total.
 //the first process is the father, we'll use it to comunicate to the user the end of the program.
 //the second process is the one who'll call exec ps
 //the third process is the zombie.

 //first son created;
 if((childpid = fork()) < 0) {
 	perror("FORK");
	exit(EXIT_FAILURE);
 } else if (childpid == 0) _exit(EXIT_SUCCESS); //now process one is a zombie, because the father
						//dont know that the son is terminated.
 
 //if the father, create another son
 if(childpid > 0) {
 	if((childpid = fork()) < 0) {
	perror("FORK");
	exit(EXIT_FAILURE);
	} else if (childpid > 0) waitpid(childpid, NULL, 0); 
          //we need to wait this specific process, the one we just create, because if we use
	  //wait instead of waitpid, we'll return the first process terminated, and in this case
          //the wait will return immediatly the zombie son, waiting for a wait. (lmao)
 }

 //now in the childpid is memorized the last PID of the last process created. 
 //the first one is a zombie.

 //let's call an execlp to know the termination status of the process.
 
if(childpid == 0) { //if the son
	execlp("ps","ps",NULL); //then exec 
   }

 //now we will terminate the zombie son still active:
 printf("I'm the father ID: %d and i'm gonna terminate my zombie son:\n",getpid());

 wpid = wait(NULL); //getting the id of the zombie son and terminating definitely 

 printf("I terminated the zombie son with ID: %d, now exiting.\n",wpid);
 
 exit(EXIT_SUCCESS); //exiting

}
