#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

//Program that will create a zombie process and 
//the main process will recognize and kill it

int main () {

 int childpid = 0;
 int status = 0;
 pid_t zombiepid;

 printf("I'm the father %d and i'm about to create a child\n", getpid());
 //forking the process into CHILD ONE
 childpid = fork();

 //now we'll create a zombie 
 if(childpid == 0) {
  printf("I'm the son %d and i'm going to be a zombie.\n",getpid());
  exit(EXIT_SUCCESS);
 }

 //sleeping for checking in the PS for the zombie process before it terminates
 sleep(5); 

 //now we will recognize a zombie 
 if((zombiepid = waitpid(-1,&status,0)) < 0) {
  perror("WAITPID ERROR:");
  _exit(EXIT_FAILURE);
 } 

 printf("I detected my zombie son %d: NOW IS TERMINATED.\n",zombiepid);
 //sleep to check again the ps 
 sleep(5);
 exit(0);

}
