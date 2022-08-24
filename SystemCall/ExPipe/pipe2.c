#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int arr[] = {1, 2, 3, 4, 1, 2};
    int arrSize = sizeof(arr) / sizeof(int);
    int fd[2];
    int start, end;
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
        start = 0;
        end = arrSize / 2;
    } else {
        start = arrSize / 2;
        end = arrSize;
    }
    int i = 0; 
    int sum = 0;
    for(i = start; i < end; i++) {
        sum = sum + arr[i];
    }
    printf("Somma parziale calcolata: %d\n", sum);
    if(pid == 0) {
        close(fd[0]);
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]);
    } else {
        int sumChild, totalSum;
        close(fd[1]);
        read(fd[0], &sumChild, sizeof(sumChild));
        close(fd[0]);
        totalSum = sumChild + sum;
        printf("Somma totale calcolata dal processo padre: %d\n", totalSum);
        wait(NULL);
    }
}