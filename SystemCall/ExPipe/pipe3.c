#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid;
    if(pipe(fd) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE);
    }
    if((pid = fork()) == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp("wc", "wc", "-w", NULL);
        // se l'esecuzione continua vuol dire che 
        // la execlp ha avuto qualche errore
        perror("ERROR 1");
        exit(EXIT_FAILURE);
    } else {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("ls", "ls", "-l", NULL);
        // se l'esecuzione continua vuol dire che 
        // la execlp ha avuto qualche errore
        perror("ERROR 2");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}