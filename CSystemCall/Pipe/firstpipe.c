#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//Simple program that implement a pipe between two process. 
//in this case, Father = writer, Son = reader.

#define READ 0
#define WRITE 1

char* str = "PIPE MESSAGGE BETWEEN FATHER AND SON.\n";

int main() {

 //Initialization
 pid_t childpid; 
 int pipefd[2];
 int nbyte;
 char buf[64];
 
 //creating our pipe into pipefd, and checking for errors.
 if(pipe(pipefd) == -1) {
	perror("PIPE");
	exit(EXIT_FAILURE);
 }

 //now fork:
 if((childpid = fork()) > 0) { //if father
	close(pipefd[READ]); //father is the write, so close his reading file descriptor
      /*close(pipefd[WRITE]); //do this for making the father the READER*/
 } else if(childpid == 0) {
	close(pipefd[WRITE]); //son is reader, so close his writing file descriptor
      /*close(pipefd[READ]); //do this for making the son the WRITER*/
   } else { //else error
	perror("FORK");	
	exit(EXIT_FAILURE); 
     }

 //we can simply invert the order of the data stream closing the correspective output/input 
 //of the process. ALSO, remember to change the order of the operation down there:
 //if childpid == 0 else childpid > 0, because the order is now inverted.

 //now let's execute the write and read to the pipe
 if(childpid > 0) { //if the writer father
	write(pipefd[WRITE], str, strlen(str)); //write on the pioe
	close(pipefd[WRITE]); //closing the write pipe when finished
	wait(NULL);
 } else if (childpid == 0) { //if the son
    	nbyte = read(pipefd[READ], buf, strlen(str)); //read from the pipe
	printf("Byte read: %d\nMessage: %s",nbyte, buf); //stamp info
	close(pipefd[READ]); //closing the read pipe when finished   
   }


 printf("PROCESS ID %d TERMINATED.\n",getpid());

}

