#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//Program to create a process ternary tree, given a max desidered height, the programm will
//create a ternary process tree, from 0 to a max depth level, and everytime a process will reach
//it's max depth it will stop from creating process, break from the cycle and print on the screen
//"concluded".

int main(int argc, char* argv[]) {

//Variables initializing
pid_t childpid = 0;

pid_t wpid = 0;
int tlevel = 0;
int maxdepth;
int treekind;

//if too few/many argument, exit
if(argc<=2) {
	printf("TOO FEW ARGUMENT.\n");
	exit(EXIT_FAILURE);
	}


 //print root process
 printf("ROOT PROCESS ID: %d\n",getpid());
 puts("");

 //translate max depth from char to int
 maxdepth = atoi(argv[1]);
 //translate the kind of tree we want (2 = binary, 3 = ternary ecc) from char to int
 treekind = atoi(argv[2]);

 //for i = 1 from i <= 3: here we'll create a ternary tree, starting from 1 to a value <= 3.
 //i.e: for binary we should have put <=2, for n-ary tree we put <= n.
  for(int i = 1; i <= treekind; i++) {
	 //fork now: if the father, then it should wait for the termination of the child.
         //main process will terminate last.
	 if((childpid = fork()) > 0) {
 		 //wait
		 wait(NULL);
	 } else if(childpid == 0) { //if the son
		 //we're on a new depth level; increase the value
		 tlevel++;
		 //sleep for tlevel seconds 
		 sleep(tlevel);
		 //we need to reinitialize the i to 0: because the process will increase it 
		 //to 1 when exiting from this else if and going to fork to another process:
		 //so we simulate the first fork, when the main process entered and his i was = 1.
		 //otherwise, if not initializing again, we will not have a ternary tree.
		 i = 0;
 		 //if the desired depth is reached, break from for and return
		 if(tlevel >= maxdepth) break; 
		 }			       
  }
 
 //print info
 printf("PROCESS ID %d - FATHER ID %d - AT LEVEL %d CONCLUDED.\n",getpid(),getppid(),tlevel);
 return(0);

}
