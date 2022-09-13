#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int sig;

static void handler(int signo) {
	signo = sig;
}

int main(int argc, char* argv[]) {
	pid_t child;
	pid_t child2;
	int num = atoi(argv[1]);
	printf("Numero ricevuto: %d\n", num);
	struct sigaction sa;
	sa.sa_handler = &handler;
	sigaction(SIGUSR1, &sa, NULL);
	if(mkfifo("fifo", 0777) == -1) {
		if(errno != EEXIST) {
			perror("FIFO EXIST");
			exit(EXIT_FAILURE);
		}
	}
	int fd = open("fifo", O_RDWR);
	child = fork();
	if(child < 0) {
		perror("FORK");
		exit(EXIT_FAILURE);
	} else if(child == 0) {
		// child process
		int x = num;
		pause();
		if(write(fd, &x, sizeof(int)) == -1) {
			perror("WRITE");
			exit(EXIT_FAILURE);
		}
	}
	child2 = fork();
	if(child < 0) {
		perror("FORK");
		exit(EXIT_FAILURE);
	} else if(child2 == 0) {
		// child2 process
		int y = -1;
		if(read(fd, &y, sizeof(int)) == -1) {
			perror("READ");
			exit(EXIT_FAILURE);
		}
		printf("Valore di y: %d\n", y);
		close(fd);
	}

	// main process
	if(num % 2 == 0) {
		kill(SIGUSR1, child);
		kill(SIGUSR1, child2);
	} else {
		printf("Il numero inserito non è divisibile per 2: %d\n", num);
		kill(SIGKILL, child);
		kill(SIGKILL, child2);
	}
	wait(NULL);
	wait(NULL);
}
