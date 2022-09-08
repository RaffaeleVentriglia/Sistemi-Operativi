/*
    Realizzare un programma C e Posix sotto Linux che con l'uso dei
    segnali sincronizzi un processo padre ed un processo figlio che
    scrivono e leggono, rispettivamente, un numero intero alla volta
    (da 1 a n, dove n è passato da riga di comando) nella prima
    posizione di un file temporaneo opportunamente creato
*/

// DA AGGIUSTARE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

int scritt = 0;

static void handler(int signo) {
	
}

int main(int argc, char* argv[]) {
	struct sigaction sa;
	sa.sa_handler = &handler;
	sigaction(SIGUSR1, &sa, NULL);
	pid_t child;
	int n = atoi(argv[1]);
	int fd;
	printf("N ricevuto: %d\n", n);
	if((fd = open("temp", O_RDWR | O_CREAT, 0777)) == -1) {
		perror("OPEN");
		exit(EXIT_FAILURE);
	}
	if((child = fork()) < 0) {
		perror("FORK");
		exit(EXIT_FAILURE);
	} else if(child == 0) {
		// Child process
		int i, num = 0, num2;
		if(scritt == 0) {
			for(i = 0; i < n; i++) {
				if(read(fd, &num2, sizeof(int)) == -1) {
					perror("READ");
					exit(EXIT_FAILURE);
				}
				printf("Contenuto del file: %d\n", num2);
				num++;
				lseek(fd, 0, SEEK_SET);
				if(write(fd, &num, sizeof(int)) == -1) {
					perror("WRITE");
					exit(EXIT_FAILURE);
				} else {
					scritt = 1;
					kill(getppid(), SIGUSR1);
					pause();
				}
			}
		} else {
			pause();
		}
	} else {
		// Parent process
		int i, num = 0, num2;
		if(scritt == 1) {
			for(i = 0; i < n; i++) {
				if(read(fd, &num2, sizeof(int)) == -1) {
					perror("READ");
					exit(EXIT_FAILURE);
				}
				printf("Contenuto del file: %d\n", num2);
				num++;
				lseek(fd, 0, SEEK_SET);
				if(write(fd, &num, sizeof(int)) == -1) {
					perror("WRITE");
					exit(EXIT_FAILURE);
				} else {
					scritt = 0;
					kill(child, SIGUSR1);
					pause();
				}
			}
		} else {
			pause();
		}
	}
	wait(NULL);
	return 0;
}
