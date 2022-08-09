#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//Program to simulate bash that create 3 son process and let them simulate the pipe command
//ls | sort | grep "Pattern".

#define READ 0
#define WRITE 1

int isA = 0;
int isB = 0;
int isC = 0;

void close_entire_pipe(int []);

int main(int argc, char* argv[]) {

 //Initialization
 pid_t childpid = 0, childpidA = 0, childpidB = 0;
 int pfd[2]; //pipe for the process A and B
 int spfd[2]; //pipe for the process B and C

 //initializing the pipe
 if(pipe(pfd) == -1) {
	perror("PIPE");
	exit(EXIT_FAILURE);
 }

 //initializing the son pipe
 if(pipe(spfd) == -1) {
	perror("PIPE");
	exit(EXIT_FAILURE);
 }

 //now creating three process:
 if((childpid = fork()) == 0) { //if is A son
 	isA = 1; //is A
 	close(pfd[READ]); //closing the read pipe because process A will write 
	close(spfd[WRITE]); //close the entire pipe for process B/C
	close(spfd[READ]);//same as above
 	printf("Process A (ls) alive.\n");
 } else if (childpid > 0) { //if the father
  	 childpidA = childpid; //assigning current childpid to childpidA 
	 childpid = 0; //resetting the childpid
	 if((childpid = fork()) == 0) { //if the son B
	 	isB = 1;
		close(pfd[WRITE]); //close write pipe to process A (because we're reading from A)
		close(spfd[READ]); //close read pipe to process C (because we're writing to C)
	 	printf("Process B (sort) alive.\n");
		sleep(1); //waiting for the A completion
	 } else if (childpid > 0) { //if the father again
		 childpidB = childpid; //assigning current childpid to childpidB
		 childpid = 0; //resetting the childpid
		 if((childpid = fork()) == 0) { //if the son C
		 	isC = 1;
			close_entire_pipe(pfd); //close the entire pfd pipe
			close(spfd[WRITE]); //close pipe write because process B will write
		 	printf("Process C (grep) alive.\n");
			sleep(1); //wait for the B completion
		 } 
	   } 
  } 


 //now, if the father, close all the communication channel
 if(childpid > 0) {
	close_entire_pipe(pfd); //close the entire pfd pipe
	close_entire_pipe(spfd); //close the entire spfd pipe
 }


 if(isA) { //is son A
	dup2(pfd[WRITE], STDOUT_FILENO); //create a new redirection before executing
	close(pfd[WRITE]); //close the old channel before exec
	execlp(argv[1], argv[1], NULL); //exec ls
	perror("exec"); //if executing this perror
	exit(EXIT_FAILURE); //exec failed
 } else if (isB) { //is son B
	 dup2(pfd[READ], STDIN_FILENO); //create a new redirection before executing
	 close(pfd[READ]); //close the old channel before exec
	 dup2(spfd[WRITE], STDOUT_FILENO); //create another redirection (this time for the process C)
	 close(spfd[WRITE]); //close the old channel before exec
	 execlp(argv[2], argv[2], NULL); //execute sort
	 perror("exec"); //if executing this perror
	 exit(EXIT_FAILURE); //exec failed
   } else if (isC) { //is son C
	  dup2(spfd[READ], STDIN_FILENO); //create a new redirection before executing
	  close(spfd[READ]); //closing the old channel before exec
 	  execlp(argv[3], argv[3], argv[4], NULL); //executing grep and pattern
	  perror("exec"); //if executing this error
	  exit(EXIT_FAILURE); //exec failed
     }

 //now, this code will be executed only from the father:
 while(wait(NULL) > 0);
 printf("FATHER TERMINATED.\n");
 return(0);

}


void close_entire_pipe(int pipe[]) {
 
 close(pipe[READ]);
 close(pipe[WRITE]);

}



