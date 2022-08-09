#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

//Our program will have a main process and some subprocess to execute some command.
//the root process will wait until is first son completed a "ls -l" command, and then
//will create a new process that will sleep for 5 seconds and terminate.
//in the end, the root process will print the PID of the terminated process.

int main() {

pid_t childpid;
pid_t wpid;
int status;

 if((childpid = fork()) > 0) {
	printf("FATHER PROCESS ID %d WAITING.\n",getpid());
	wait(&status);
	} else if (childpid == 0) {
		execlp("ls","ls","-l",NULL);
		}
	
 //now the father will print "NEW SON" and create a new process that will sleep for 5 seconds.
 //the father will wait it.

 if((childpid = fork()) > 0) {
	printf("NEW SON ABOUT TO BE CREATED\n");
	wpid = wait(&status);
	} else if (childpid == 0) {
		sleep(5);
		printf("I'M THE SON PROCESS ID: %d AND SLEEPED FOR 5 SECS.\n",getpid());
		_exit(EXIT_SUCCESS);
	}

 //now the father will print the pid of the last process terminated 
 printf("I'M THE FATHER %d AND THE LAST PROCESS PID TERMINATED IS: %d",getpid(),wpid);


}
