#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	pid_t pid[n];
	int i;
	for(i = 0; i < n; i++) {
		pid[i] = fork();
		if(pid[i] < 0) {
			perror("FORK");
			exit(EXIT_FAILURE);
		} else if(pid[i] == 0) {
			// child process
			printf("Figlio: %d\n", getpid());
			return 0;
		}
	}
	wait(NULL);
	printf("Numero di processo: %d\n", getpid());
	return 0;
}
