#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int isOdd = 0;
int isEven = 0;

void safe_pipe(int []);
void close_entire_pipe(int []);
void fork_error();

int main() {

 //initialization
 int ofd[2], efd[2], rofd[2], refd[2]; //defining pipe communications
 int n, readn = 0, sum = 0, tnum = 0;
 pid_t childpid = 0;

 //starting our program defining the pipe to open:
 safe_pipe(ofd);
 safe_pipe(efd);
 safe_pipe(rofd); 
 safe_pipe(refd);

 //forking the father into odd son, and even son

 if((childpid = fork()) == 0) { //if the son
	close(ofd[WRITE]); //close write: here the son must only read
	close(rofd[READ]); //close read: here the son must only write
	close_entire_pipe(efd); //close the entire pipe, we don't need that
	close_entire_pipe(refd); //close the entire pipe, we don't need that
	isOdd = 1; //is odd
	printf("ODD SON CORRECTLY DEFINED PIPES.\n");
 } else if (childpid > 0) {
	childpid = 0; //resetting chilpid
	if((childpid = fork()) == 0) { //if the son
		close(efd[WRITE]); //close write: here the son must only read
		close(refd[READ]); //close read: here the son must only write	
		close_entire_pipe(ofd); //close the entire pipe, we don't need that
		close_entire_pipe(rofd); //close the entire pipe, we don't need that
		isEven = 1; //is even
		printf("EVEN SON CORRECTLY DEFINED PIPES.\n");
	} else if (childpid > 0) { //if father again
 	  	close(ofd[READ]); //close read: here the father must only write
		close(rofd[WRITE]); //close the write: here the father must only read
		close(efd[READ]); //close the read: here the father must only write
		close(refd[WRITE]); //close the write: here the father must only read
		printf("FATHER CORRECTLY DEFINED PIPES.\n");
		sleep(1); 
	  } else fork_error();
   } else fork_error();



 if(childpid > 0) {
	while(1) {
		printf("INSERT THE NUMBER (NEGATIVE TO TERMINATE): "); 	
		scanf("%d",&n);
		fflush(stdin);

		if(n < 0) { //if the number entered is negative
			write(ofd[WRITE], &n, sizeof(int)); //write to odd son
			write(efd[WRITE], &n, sizeof(int)); //write to even son
			close(ofd[WRITE]); //we can close the descriptor, nothing more to do
			close(efd[WRITE]); //SAME ^
			break; //break from the cycle

		} else if (n % 2 == 0) { //if the number entered is odd
			write(ofd[WRITE], &n, sizeof(int)); //write to odd son		  

		  } else { //else, is even
		    	write(efd[WRITE], &n, sizeof(int)); //write so even son

		    }
	} //end while

 //if the father exited, now need to read from their son the partial sum:
 read(rofd[READ], &readn, sizeof(int)); //read the partial sum of the odd son into readn
 close(rofd[READ]); //close the fd
 sum += readn; //add it to the total sum
 read(refd[READ], &readn, sizeof(int)); //read the partial sum of the even son into readn
 close(refd[READ]); //close the fd
 sum += readn; //add it to the total sum
 printf("TOTAL SUM IS: %d\n",sum); 

 } else if (isOdd) {
	while(1) {
		read(ofd[READ], &readn, sizeof(int)); //read the number 
		if(readn < 0) break;
		else {
			sum += readn;
			tnum++;	
		}
	}

    //if the son breaked, he need to pass the actual partial sum to the father
    printf("ODD SON TOTAL NUMBER READ: %d\n",tnum);
    write(rofd[WRITE], &sum, sizeof(int));
    close(rofd[WRITE]); //close the fd
   
   } else if (isEven) {
	while(1) {	
		read(efd[READ], &readn, sizeof(int)); //read the number 
		if(readn < 0) break;
		else {
			sum += readn;	
			tnum++;		
		}
	}

      //if the son breaked, he need to pass the actual partial sum to the father
      printf("EVEN SON TOTAL NUMBER READ: %d\n",tnum);
      write(refd[WRITE], &sum, sizeof(int)); 
      close(refd[WRITE]); //close the fd
	
     }

 if(childpid > 0) wait(NULL);
 else sleep(1); 
 printf("PROCESS %d TERMINATED.\n", getpid());

}


void safe_pipe(int fdpipe[]) {

 if(pipe(fdpipe) == -1) {
	perror("PIPE");
	exit(EXIT_FAILURE);	
 }

}


void close_entire_pipe(int fdpipe[]) {

 close(fdpipe[READ]);
 close(fdpipe[WRITE]);

}

void fork_error() {
 
 perror("FORK");
 exit(EXIT_FAILURE);
		
}
