#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1	

int isA = 0;
int isB = 0;

int main(int argc, char* argv[]) {

 //Initializing
 pid_t childpid = 0;
 int pipefd[2];

 //initializating the pipe for all the process
 if(pipe(pipefd) == -1) {
	perror("PIPE");
	exit(EXIT_FAILURE);
 }
 
 //We're going to fork two process from the process father: 
 //one for argument.

 if((childpid = fork()) == 0) { //if the son A
 	isA = 1; //is process A
	close(pipefd[READ]); // process A is the WRITER.
 } else if(childpid > 0) { //if the father
	 if((childpid = fork()) == 0) {//if the son B
		isB = 1; //is process B
		close(pipefd[WRITE]); //process B is the READER.
	 } else if (childpid > 0) { //again, if the father
		close(pipefd[READ]); //we dont need open pipe anymore.
		close(pipefd[WRITE]); //we dont need open pipe anymore.
          } else exit(EXIT_FAILURE);
	
   } else exit(EXIT_FAILURE);

 //now, let's define the son process action:

 if(isA) { //if is the process A
 	dup2(pipefd[WRITE], STDOUT_FILENO); //redirect stdout to the pipe write.
 	close(pipefd[WRITE]); //close the pipe write
 	execlp(argv[1], argv[1], argv[2], NULL); //execute first and second command ("ls" and "-R")
 	perror("EXEC"); //if process can print this exec has failed
 	exit(EXIT_FAILURE); //exit
 } else if (isB) { //if is the process B
	sleep(1); //give the time to A to be executed
	dup2(pipefd[READ], STDIN_FILENO); //duplicate the existent PIPE READ CHANNEL to point to STDIN.
	close(pipefd[READ]); //close the read channel before EXEC
 	execlp(argv[3], argv[3], argv[4], NULL); //going to execute grep and pattern
	perror("EXEC"); //IF THE process can print this, exec has failed
	exit(EXIT_FAILURE); //exit 
  }


 while(wait(NULL) > 0); //wait all the son terminating
 printf("PROCESS FATHER TERMINATED.\n");
 return(0);
 
}
