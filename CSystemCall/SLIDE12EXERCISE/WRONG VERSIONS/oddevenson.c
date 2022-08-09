#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

//Program that will manage: Father, an odd son, and a even son.
//THe father will scan number from keyboard: if the number is odd, 
//he will send it to his odd son, that will make the partial sum of the new
//number with all the number sent before. same for the even son with even numbers.
//if a number scanned is non-positive, both the even/odd son will send to the father their
//partial sum, and the father will sum the numbers and print the result to video.

int isOdd = 0;
int isEven = 0;


int main() {

 //initialization
 int n, readn = 0, sumsofar = 0; //n = number to scan, readn = number to read, sumsofar = total sum number
 int oddpipefd, evenpipefd; //using the fd for communication FATHER WRITER / SONS READER
 int roddpipefd, revenpipefd; //if necessary (when a non-positive number occurs) FATHER IS READER SONS ARE WRITERS.
 pid_t childpid = 0;

 
 //calling the pipe creation script
 if(fork() == 0) { //if the son
	execl("createfifos", "./createfifos" ,NULL); //going the son to die executing the create fifos script	
 } else wait(NULL);


 //Now creating the son 
 if((childpid = fork()) == 0) { //if the son
 	if((oddpipefd = open("oddpipe", O_RDONLY, 0777)) < 0) { //open the odd pipe in read. if the odd fd is < 0
		perror("OPEN"); //then there's an error
		exit(EXIT_FAILURE); //exit
	}//endif
	
	if((roddpipefd = open("roddpipe", O_WRONLY, 0777)) < 0) { //open the oddpipe to write too
		perror("OPEN");
		exit(EXIT_FAILURE);
	}


 	isOdd = 1; //identifying odd son
	printf("SON ID %d: ODD PIPE READ/WRITE OPENED OK\n",getpid());	

 } else if (childpid > 0) { //if the father
	childpid = 0; //reset childpid
	if((childpid = fork()) == 0) { //if the son
 		if((evenpipefd = open("evenpipe",O_RDONLY, 0777)) < 0) {
			perror("OPEN");	
			exit(EXIT_FAILURE);
		}//endif
	
		if((roddpipefd = open("revenpipe", O_WRONLY, 0777)) < 0) { //open the oddpipe to write too
			perror("OPEN");
			exit(EXIT_FAILURE);
		}
		isEven = 1; //identifying even son
		printf("SON ID %d: EVEN PIPE READ/WRITE OPENED OK\n",getpid());	

	} else if(childpid > 0) { //if the father again
		if((oddpipefd = open("oddpipe", O_WRONLY, 0777)) < 0) {
			perror("OPEN");
			exit(EXIT_FAILURE);	
		}		

		if((evenpipefd = open("evenpipe", O_WRONLY, 0777)) < 0) {
			perror("OPEN");
			exit(EXIT_FAILURE);
		}

		if((roddpipefd = open("roddpipe", O_RDONLY, 0777)) < 0) { //open the oddpipe to write too
			perror("OPEN");
			exit(EXIT_FAILURE);
		}

		if((revenpipefd = open("revenpipe", O_RDONLY, 0777)) < 0) { //open the oddpipe to write too
			perror("OPEN");
			exit(EXIT_FAILURE);
		}	
		
		printf("FATHER ID %d: ODD PIPE READ/WRITE OPENED OK\n",getpid());
		printf("FATHER ID %d: EVEN PIPE READ/WRITE OPENED OK\n",getpid());		

	  } else {
		perror("FORK");
		exit(EXIT_FAILURE);
 	   } //end else
   } else {
	perror("FORK");
	exit(EXIT_FAILURE);
     }



 //writing phase
  if(childpid > 0) {
 	sleep(1);
 	while(1) {
		printf("Insert a number (-1 to finish): ");
		scanf("%d",&n);
		
		//write the number on the pipes
		if(n < 0) {
			write(oddpipefd, &n, sizeof(int));
			write(evenpipefd, &n, sizeof(int));
			break; 
		}
		else if(n % 2 == 0) { //if the number is odd
			write(oddpipefd, &n, sizeof(int)); //write on the oddpipe the number			
	
		} else {
		  	write(evenpipefd, &n, sizeof(int)); //write on the evenpipe the number
		  } 
	}

 //if the cycle breaked:

 printf("waiting roddpipe..\n");
 read(roddpipefd, &readn, sizeof(int));
 sumsofar+=readn;
 printf("ODD SUMSOFAR = %d \nwaiting revenpipe..\n", sumsofar);
 fflush(stdin);
 read(revenpipefd, &readn, sizeof(int));
 printf("EVEN SUMSOFAR = %d\n", sumsofar);
 sumsofar+=readn;
 printf("TOTAL SUM OF NUMBER WRITTEN: %d\n",sumsofar);	

 } else if (isOdd) {
	while(1) {
		read(oddpipefd, &readn, sizeof(int));
		printf("ODD SON - NUMBER RECEIVED: %d\n",readn);
		if(readn < 0) break;
		else {
			sumsofar += readn;	
			printf("ODD %d\n",sumsofar);		
		}

	}
 printf("ODD - WRITING INTO PIPE\n");	
 write(roddpipefd, &sumsofar, sizeof(int));

   } else if (isEven) {
	while(1) {
		read(evenpipefd, &readn, sizeof(int));
		printf("EVEN SON - NUMBER RECEIVED: %d\n",readn);
		if(readn < 0) break;
		else {
			sumsofar += readn;	
			printf("EVEN %d\n",sumsofar);		
		}	
	}	
     sleep(1);
     printf("EVEN- WRITING INTO PIPE\n");
     write(revenpipefd, &sumsofar, sizeof(int));
	
     }



 


}

