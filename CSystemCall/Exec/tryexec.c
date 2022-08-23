#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

//we'll execute the command ls -s with both execv and execl.
//also we'll use execvp and execlp.

int main(int argc, char* argv[]) {

int status = 0;
int childpid = 0;

if((childpid = fork()) < 0) {
 perror("FORK");
 exit(EXIT_FAILURE);
} 
 
 //first: execute the command with execl
 if(childpid == 0) {
 printf("I'm the son process with ID: %d\n",getpid());
 printf("And i'm going to execute %s command with EXECL.\n", argv[1]);
 //execl is useful ONLY when we EXACTLY KNOW how many argument 
 //we need to execute. this time ls -l, so TWO argument.
 execl("/bin/ls", argv[1], argv[2], argv[argc]); //NB: argv[argc] = NULL
 } puts("");
 
 //if the parent get executed first, then make it wait
 if(wait(&status) < 0) {
 perror("WAIT");
 exit(EXIT_FAILURE);
 }
 //---------------- 

 //second: execute the command with execv, fork again
 if((childpid = fork()) < 0) {
 perror("FORK");
 exit(EXIT_FAILURE);
 }
 
 //now let the child process to execute execv
 if(childpid == 0) {
 printf("I'm the son process with ID: %d\n",getpid());
 printf("And i'm going to execute %s command with EXECV.\n", argv[1]);
 //we're going to use execv: we dont need to specificate all the
 //argument as a list, but all we need to do is pass the vector of
 //commands, terminated by NULL. argv[argc] contains NULL.
 execv("/bin/ls", argv+1);
 } puts("");
 
 //if the parent got executed first, then make it wait
 if(wait(&status) < 0) {
 perror("WAIT");
 exit(EXIT_FAILURE);
 }
 //----------------
 
 //third: execute the command with execlp:
 if((childpid = fork()) < 0) {
 perror("FORK");
 exit(EXIT_FAILURE);
 } 

 //now let the child process to handle the execlp
 if(childpid == 0) {
 printf("I'm the process with ID: %d\n",getpid());
 printf("And i'm going to execute %s command with EXECLP.\n", argv[1]);
 //we can pass the argument file as the name of the command we want to execute:
 //execlp will search in any of the folder cointained into variable $PATH if there
 //is something called like that.
 execlp(argv[1], argv[1], argv[2], argv[argc]);
 } puts("");

 //make the parent wait if necessary
 if(wait(&status) < 0) {
 perror("WAIT");
 exit(EXIT_FAILURE);
 }
 //---------------

 //fourth: lets create a child process
 if((childpid = fork()) < 0) {
 perror("FORK");
 exit(EXIT_FAILURE);
 }

 //now let the children handle the execvp command:
 if(childpid == 0) {
 printf("I'm the process with ID: %d\n",getpid());
printf("And i'm going to execute %s command with EXECVP.\n", argv[1]);
 //we're going to call execvp, and we need only to pass two argument: name of the command
 //we want to execute, and the list of command argument trough argv.
 execvp(argv[1], argv+1); 
 } 

 //if the parent will be executed before, let him wait
 if(wait(&status) < 0) {
 perror("WAIT");
 exit(EXIT_FAILURE); 
 } puts("");
 
 //end of the program
 printf("I'm the process father with ID: %d\n",getpid());
 printf("Now i'll terminate the program.\n");
 exit(EXIT_SUCCESS);


}



