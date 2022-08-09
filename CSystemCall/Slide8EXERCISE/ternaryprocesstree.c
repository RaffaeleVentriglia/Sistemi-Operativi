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
pid_t root_id = 0;
pid_t wpid = 0;
int tlevel = 0;
int maxdepth = atoi(argv[1]); //max depth is specified in the FIRST ARGUMENT level
int treekind = atoi(argv[2]); //kind of tree is specified in the SECOND ARGUMENT
 
 //if too few/many argument, exit
 if(argc<=2) {
	printf("TOO FEW ARGUMENT.\n");
 	exit(EXIT_FAILURE);
 }


 //getting id of the root
 root_id = getpid();
 printf("ROOT FATHER ID: %d\n",root_id);

 //we need to iterate maxdepth time: from i = 1 to minor or equal to maxdepth, we choose i = 1 to take track 
 //of the level we're currently analyzing, later we'll explain why.
 for(int i = 1; i<=maxdepth; i++) {
	//for j = 0 to treekind, because we need to fork treekind process. in this case, treekind = 3, 
	//so we'll fork 3 times having 3 processes forming a ternary tree. (each process node got 3 child)
	for(int j = 0; j<treekind; j++) {
			//if son, then break the cycle: we dont want a child process to fork.
			if((childpid = fork()) == 0) {
				break;
			}
  	}	
	
	//now, if it's the parent, assign to it's tlevel value i-1, because this is the current level 
        //he's in actually. (because a parent breaking its cycle never increase i for the next one)
 	if(childpid > 0) {
		tlevel = i-1;	
		break;
	}
	//else, if the son, then the actual level is just i, because a son got a level more than it's parent.
	//also, let him sleep maxdepth seconds (1 to maxdepth)
	else if (childpid == 0) {
		tlevel = i;
		sleep(maxdepth);
	}
 }

 //if a parent break, then he need to wait until ALL OF HIS SONS finished. 
 while(wait(NULL) > 0);
 printf("PROCESS ID %d - FATHER ID %d - AT LEVEL %d CONCLUDED.\n",getpid(),getppid(),tlevel);
 return(0);

}
