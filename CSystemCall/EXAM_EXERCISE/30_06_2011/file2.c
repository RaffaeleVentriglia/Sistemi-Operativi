#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define READ 0
#define WRITE 1

void close_entire_pipe(int*);
void print_fork_error();

int main(int argc, char* argv[]) {

 int pipe_AB[2];
 int pipe_BC[2];
 int is_cat = 0, is_grep = 0, is_wc = 0;
 pid_t pid; 

 if(pipe(pipe_AB) == -1) {
	perror("pipe");
	exit(EXIT_FAILURE);	
 }

 if(pipe(pipe_BC) == -1) {
	perror("pipe");	
	exit(EXIT_FAILURE);	
 }


 if((pid = fork()) == 0) {
	is_cat = 1;
	close(pipe_AB[READ]);
	close_entire_pipe(pipe_BC);
 } else if(pid > 0) {
		if((pid = fork()) == 0) {
			is_grep = 1;
			close(pipe_AB[WRITE]);
			close(pipe_BC[READ]);
		} else if(pid > 0) {
				if((pid = fork()) == 0) {
					is_wc = 1;
					close_entire_pipe(pipe_AB);	
					close(pipe_BC[WRITE]);
				} else if (pid == -1) print_fork_error();			
		} else if (pid == -1) print_fork_error();
 } else if (pid == -1) print_fork_error();

 
 if(pid > 0) {
	close_entire_pipe(pipe_AB);
	close_entire_pipe(pipe_BC);
	while(wait(NULL) != -1);
 }

 if(is_cat) {
	dup2(pipe_AB[WRITE], STDOUT_FILENO);
	close(pipe_AB[WRITE]);
	execlp(argv[1], argv[1], argv[2], NULL);
	perror("EXEC");	
	exit(EXIT_FAILURE); 
 } else if(is_grep) {
	dup2(pipe_AB[READ], STDIN_FILENO);
	dup2(pipe_BC[WRITE], STDOUT_FILENO);
	execlp(argv[3], argv[3], argv[4], NULL);
	perror("EXEC");
	exit(EXIT_FAILURE);	
   } else if(is_wc) {
	dup2(pipe_BC[READ], STDIN_FILENO);
	close(pipe_BC[READ]);
	execlp(argv[5], argv[5], argv[6], NULL);
	perror("EXEC");
	exit(EXIT_FAILURE); 
     }

 printf("PID %d: DONE.\n",getpid());
 return(0);

}


void close_entire_pipe(int* pipe) {
	
 close(pipe[READ]);
 close(pipe[WRITE]);

}

void print_fork_error() {
 
 perror("FORK");
 exit(EXIT_FAILURE);

}






