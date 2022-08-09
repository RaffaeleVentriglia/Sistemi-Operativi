#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define READ 0
#define WRITE 1

void print_fork_error();

int main(int argc, char* argv[]) {

 pid_t pid;
 int pipe_AB[2], pipe_BC[2];
 int isCat = 0;
 int isGrep = 0;
 int isWC = 0;

 if(pipe(pipe_AB) == -1) {
	perror("pipe ab");
	exit(EXIT_FAILURE);	
 }

 if(pipe(pipe_BC) == -1) {
	perror("pipe bc");
	exit(EXIT_FAILURE);
 } 

 if((pid = fork()) == 0) {
	isCat = 1;
	close(pipe_AB[READ]);
	close(pipe_BC[WRITE]);
	close(pipe_BC[READ]);
	printf("Cat process alive.\n");
 } else if (pid > 0) {
	if((pid = fork()) == 0) {
		isGrep = 1;
		close(pipe_AB[WRITE]);
		close(pipe_BC[READ]);	
		printf("Grep process alive.\n");
	} else if (pid > 0) {
		if((pid = fork()) == 0) {
			isWC = 1;
			close(pipe_AB[READ]);
			close(pipe_AB[WRITE]);
			close(pipe_BC[WRITE]);
			printf("WC process alive.\n");
		} else if (pid == -1) 
			print_fork_error();
	} else print_fork_error();
 } else print_fork_error();

 if(pid > 0) {
	close(pipe_AB[READ]);
	close(pipe_AB[WRITE]);
	close(pipe_BC[WRITE]);
	close(pipe_BC[READ]);
	while(wait(NULL) != -1); 
}

 if(isCat) {
	dup2(pipe_AB[WRITE], STDOUT_FILENO);
	close(pipe_AB[WRITE]);
	execlp(argv[1], argv[1], argv[2], NULL);
	perror("execlp isCat");
	exit(EXIT_FAILURE);
  } else if(isGrep) {
	dup2(pipe_AB[READ], STDIN_FILENO);
	close(pipe_AB[READ]);
	dup2(pipe_BC[WRITE], STDOUT_FILENO);
	close(pipe_BC[WRITE]);	
	execlp(argv[3], argv[3], argv[4], NULL);
	perror("execlp isCat");
	exit(EXIT_FAILURE);
   } else if (isWC) {
	dup2(pipe_BC[READ], STDIN_FILENO);
	close(pipe_BC[READ]);
	execlp(argv[5], argv[5], argv[6], NULL);
	perror("execlp isCat");
	exit(EXIT_FAILURE);
    }
 
 printf("Process parent terminated.\n");
 return(0);

}

void print_fork_error() {

 perror("fork");
 exit(EXIT_FAILURE);
	

}
