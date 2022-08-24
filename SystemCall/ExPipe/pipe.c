#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd[2];
    // fd[0] read
    // fd[1] write
    if(pipe(fd) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if(pid == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        close(fd[0]);
        int x;
        printf("Inserire un numero: ");
        scanf("%d", &x);
        if(write(fd[1], &x, sizeof(int)) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
    } else {
        close(fd[1]);
        int y;
        if(read(fd[0], &y, sizeof(int)) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        y = y*y*y;
        close(fd[0]);
        printf("Cubo del valore ricevuto dal processo figlio: %d\n", y);
    }
}