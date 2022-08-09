#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//global variables:
int glob = 5;
char buffer[] = "GLOBAL BUFFER CONTENT\n";

int main () {

 pid_t childpid;
 int var = 40; //automatic variable = synonim of local variable

//writing on the STDIO the content of buffer
 if(write(STDOUT_FILENO, buffer, sizeof(buffer)) != sizeof(buffer)) {
 perror("WRITE ERROR");
 exit(EXIT_FAILURE);
 } 

 printf("PRINTF BEFORE FORK:\n");
 //flushing the buffer, otherwise it will remain dirty in the stamp
 //NB: this is useful only when printing information in something different
 //than the terminal, for example rediretion to a file. 
 //we dont need to empty the buffer in case of using a terminal
 fflush(stdout);
 
 //if the FORK fails, then print the error and exit
 if((childpid = fork()) < 0) {
 perror("FORK ERROR");
 exit(EXIT_FAILURE); 
 } else if(childpid == 0) { //else, if the fork works and this is the child process
    
    glob++;
    var++;
    printf("I'M CHILD PROCESS %d, GLOB = %d / VAR = %d\n",getpid(),glob,var);
    exit(EXIT_SUCCESS);	
   } 
 
 printf("I'M FATHER PROCESS %d, GLOB = %d / VAR = %d\n",getpid(),glob,var);

}
