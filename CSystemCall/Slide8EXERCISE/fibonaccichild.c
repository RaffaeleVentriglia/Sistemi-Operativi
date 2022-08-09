#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

//This program will create a son process and the new son will execute FIBONACCI_FUNCTION, with 
//n<=12. the son process will return a termination value equal to the fibonacci number calculated.
//if the termination/exit status will be < 50, the main process will create another son and 
//execute ls -al a.out. then print a message and get terminated. 
//if exit status > 50, print a message and get terminated.


int FIBONACCI_FUNCTION(int n);


int main(int argc, char* argv[]) {

pid_t childpid;
pid_t wpid;
int status;
int nfib;
int e_status;
int fterm;
//assigning to the program name the name of the executed program + 2 character from left 
//(skipping ./)
char* pname = argv[0]+2;

 //at least/mininimum 2 argument.
 if(argc!=2) {
	printf("TOO FEW/MANY ARGUMENTS.\n");
	exit(EXIT_FAILURE);
	}

 //core: the child will execute the fibonacci function, and use the number returned as exit 
 //status. the father will wait termination of the son. 
 if((childpid = fork()) > 0) { //if father
	wpid = wait(&status);
	} else if (childpid == 0) { //if son
		nfib = atoi(argv[1]);
		e_status = FIBONACCI_FUNCTION(nfib);
		_exit(e_status);
		} else { //if failed
		     perror("FORK ERROR");
		     exit(EXIT_FAILURE);
		     }
		
 //if the termination was successful, asssign the exit status to fterm, else exit.
 if(!WIFEXITED(status)) {
	perror("ANORMAL TERMINATION");
	exit(EXIT_FAILURE);
	} else fterm = WEXITSTATUS(status);

 //if fterm > 50, just print a message and exit, else create another son and execute ls -al a.out,
 //then print a message and exit.

 if(fterm > 50) {
	printf("FTERM: %d > 50 - NOW EXITING.\n",fterm);
	exit(EXIT_SUCCESS); 
 } else { 
	printf("FTERM: %d < 50 - CREATING SON\n",fterm);
	if((childpid = fork()) > 0) {
		wait(NULL);
		} else if(childpid == 0) {
			
			execlp("ls", "ls", "-al", pname, NULL);
			}
	}
	
 printf("I'M THE FATHER %d AND I WILL NOW TERMINATE.\n",getpid());
 exit(EXIT_SUCCESS);


}


int FIBONACCI_FUNCTION(int n) {

 if(n == 0|| n == 1) 
		return n;

 return FIBONACCI_FUNCTION(n-1) + FIBONACCI_FUNCTION(n-2);

}





