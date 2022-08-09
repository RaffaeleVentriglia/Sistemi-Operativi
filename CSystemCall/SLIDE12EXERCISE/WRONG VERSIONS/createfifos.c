#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {

 //initialization:


 //CHECKING/CREATION PHASE FOR ODD PIPE
 if(access("oddpipe", F_OK) == 0) { //if we access to the file and check it exists
	printf("ODD PIPE CHECK: OK\n"); //print the message
 } else { //otherwise
	if(mkfifo("oddpipe",0777) == 0) { //if the creation of the pipe is successful
		printf("ODD PIPE CREATION: OK\n"); //print the message
	} else { //otherwise
		perror("PIPE"); //error
		exit(EXIT_FAILURE); //exit
	  }
   }

 //CHECKING/CREATION PHASE FOR EVEN PIPE
 if(access("evenpipe", F_OK) == 0) { //if we access to the file and check it exists
	printf("EVEN PIPE CHECK: OK\n"); //print the ok message
 } else { //otherwise
   	if(mkfifo("evenpipe",0777) == 0) { //if the creation of the fifo is successful
		printf("EVEN PIPE CREATION: OK\n"); //print the message
        } else { //otherwise  
		perror("PIPE"); //error
		exit(EXIT_FAILURE); //exit
	  }
   }



}
