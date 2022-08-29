#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
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
        // child process
        close(fd[0]);
        srand(time(NULL));
        int n = rand() % 10 + 1;
        int i;
        int arr[n];
        printf("Numero di elementi: %d\n", n);
        if(write(fd[1], &n, sizeof(int)) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
        printf("Numeri generati: ");
        for(i = 0; i < n; i++) {
            arr[i] = rand() % 20;
            printf("%d ", arr[i]);
        }
        printf("\n");
        if(write(fd[1], arr, sizeof(int)*n) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent process
        close(fd[1]);
        int n, i, somma = 0;
        if(read(fd[0], &n, sizeof(int)) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        printf("Numero ricevuto: %d\n", n);
        int arr[n];
        if(read(fd[0], arr, sizeof(int)*n) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        printf("Elementi ricevuto: ");
        for(i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        for(i = 0; i < n; i++) {
            somma += arr[i];
        }
        printf("Somma calcolata: %d\n", somma);
        wait(NULL);
    }

}