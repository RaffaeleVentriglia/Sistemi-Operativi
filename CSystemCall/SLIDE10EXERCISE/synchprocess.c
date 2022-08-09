#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

//synchronize two process father and son, to write on a file temp.out created by
//apposite function, to write one integer at time, from 1 to n where n is passed 
//by terminal.

//global variables to take track / be useful to the processes.

//count the actual number the process is writing to the file
int totalnum = 1;
//global file descriptor 
int gfd;
//bytecount
int n;
//buffer to read the str written on file
char buf[16];
//str to write on file
char str[16];


//signal handler
static void signal_handler(int);

int main(int argc, char* argv[]) {

 //Initialization:
 pid_t childpid;
 struct sigaction act,oact;
 sigset_t set;
 int argnum = atoi(argv[1]);
 int fd = 0; //file descriptor for the file 
 
 //open the file, if not existent create it 
 if((fd = open("temp.out", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
	perror("OPEN");
	exit(EXIT_FAILURE);
 }

 //global file descrpitor
 gfd = fd;

 //defining the mask for the blocked signals while in the handler
 sigfillset(&set);

 //building sigaction struct and initializing it with the associated function
 act.sa_handler = &signal_handler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;
 sigaction(SIGUSR1, &act, &oact);
 sigaction(SIGUSR2, &act, &oact);

 //now forking the process: 

 if((childpid = fork()) < 0) {
	perror("fork");
	exit(EXIT_FAILURE);
 } else if (childpid == 0) { //if son
	 totalnum = 2; //because the son will start writing/reading after the father.
	 sprintf(str, "%d", totalnum); // transforming totalnum from int to a string into str
	 sleep(1); //give the father the opportunity to be executed first
   } else if (childpid > 0) {
	  sprintf(str, "%d", totalnum); //same as above
     }

 printf("PROCESS %d ALIVE\n",getpid());
 //we need to stop 1 process to garantee the mutual esclusion access:
 //in this case, we'll stop the son process first, so we can allow the father to 
 //send a signal to him after he's done.

 //now, loop until the number reached the argument number passed by terminal:
 //this is the core of the synchronization.
 while(totalnum < argnum) {
 	if(childpid > 0) { //if the father
		pause(); //process will be restored ad the reception of a signal
		sprintf(str, "%d", totalnum); //translate the new totalnum int to a str
		kill(childpid, SIGUSR2); //send a signal to the son to unlock him
	} else if (childpid == 0) { //if the son
		 kill(getppid(), SIGUSR1); //send a signal to the father to unlock him	
		 pause(); //process will be restored ad the reception of a signal
		 sprintf(str,"%d",totalnum); //translate the new totalnum int to a str
	  }
 } //end while

 //if the father
 if(childpid > 0) {
	 wait(NULL); //dont let the son be a zombie
 } else if (childpid == 0) { //if the son
 	kill(getppid(),SIGUSR1); //unlock the father 
	sleep(1); //let the father finish his job
 }
 
 printf("PROCESS %d TERMINATED.\n",getpid());
 close(gfd); 
 return(0); //exit

}

//defining the handler
static void signal_handler(int signo) {

 if(signo == SIGUSR1) { //if the father
	//write to the file
 	if(write(gfd, &str, strlen(str)) != strlen(str)) {
		perror("WRITE");
		exit(EXIT_FAILURE);
	} 		

	//reset the offset: we want to write at every iteration in the FIRST POSITION.
	//and also read what we've written, because write move the offset when write on file.
	lseek(gfd, 0, SEEK_SET);

	while((n = read(gfd, &buf, strlen(str))) > 0) {
		if(n < 0) {	
			perror("READ");	
			exit(EXIT_FAILURE);
		} else	printf("%s - F\n",buf);
	}

	//read move the offset, so we need to reset him again
 	lseek(gfd, 0, SEEK_SET);
	
	//increase the number;
	totalnum+=2;
	
 } else if(signo == SIGUSR2) { //if the son
   	//write to the file
 	if(write(gfd, &str, strlen(str)) != strlen(str)) {
		perror("WRITE");
		exit(EXIT_FAILURE);
	} 

	//reset the offset: we want to write at every iteration in the FIRST POSITION.
	//and also read what we've written, because write move the offset when write on file.
	lseek(gfd, 0, SEEK_SET);

	//lets read from the file we wrote in the precedent write
  	while((n = read(gfd, &buf, strlen(str))) > 0) {
		if(n < 0) {	
			perror("READ");	
			exit(EXIT_FAILURE);
		} else	printf("%s - S\n",buf);
	}	

	//read move the offset, so we need to reset him again
 	lseek(gfd, 0, SEEK_SET);
	
	//increase the number;
	totalnum+=2;
	
   }

}

















