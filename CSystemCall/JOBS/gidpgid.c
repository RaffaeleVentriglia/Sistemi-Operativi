#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

pid_t childpid = 0;

childpid = fork();

//printing phase
 if(childpid == 0) {
	printf("SON ID %d - GID %d - PGID %d - FATHER %d\n", getpid(),getgid(),getpgid(0),getppid());
 } else if (childpid > 0) {
 		printf("FATHER ID %d - GID %d - PGID %d\n", getpid(),getgid(),getpgid(0));
		wait(NULL);  
		return(0);
 }

//changing PGID of the son:
 if(childpid == 0) {
	setpgid(0,getpid());
	printf("SON ID %d -GID %d - NEW PGID %d - FATHER %d\n", getpid(),getgid(),getpgid(0),getppid ());
 return(0);
 }
}
 
