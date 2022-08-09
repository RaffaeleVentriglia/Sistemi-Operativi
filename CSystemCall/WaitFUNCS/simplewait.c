#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

//Simple program that use wait function to wait a completion
//of a child process, then get his exit status and complete the execution.

int main() {
 fflush(stdout);
 int status = 0;
 int childpid = 0;
 
 printf("FATHER ID %d\n",getpid());
 //forking the main process into CHILD ONE
 childpid = fork();


 if (childpid > 0) {
	printf("I'm the father %d and i'm going to wait.\n",getpid());
	wait(&status);        	
	}



 //now it's the turn of the child son to execute the code
 if(childpid == 0) {
	printf("I'M THE SON %d\n",getpid());
	printf("SON WRITING\n");
	_exit(EXIT_SUCCESS);
	}

 printf("I'm the father and i waited till my son finished.\n");
 printf("Now we gonna see what type of termination my son got:\n");

 if(WIFEXITED(status)) printf("NORMAL TERMINATION - EXIT STATUS: %d\n",WEXITSTATUS(status));
 else if(WIFSIGNALED(status)) printf("ANOMAL TERMINATION - SIGNAL NUMBER: %d\n",WTERMSIG(status));
 else if(WIFSTOPPED(status)) printf("SON STOPPED - SIGNAL NUMBER: %d", WSTOPSIG(status));

} 

 


