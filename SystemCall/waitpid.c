#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid1, pid2;

    if((pid1 = fork()) == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(pid1 == 0) {
        // child 1 process
        sleep(4);
        printf("Esecuzione terminata (%d)\n", getpid());
        exit(EXIT_SUCCESS);
    }

    if((pid2 = fork()) == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(pid2 == 0) {
        // child 2 process
        sleep(1);
        printf("Esecuzione terminata (%d)\n", getpid());
        exit(EXIT_SUCCESS);
    }

    int pid1_res = waitpid(pid1, NULL, 0);
    printf("Ho aspettato il processo: %d\n", pid1_res);
    int pid2_res = waitpid(pid2, NULL, 0);
    printf("Ho aspettato il processo: %d\n", pid2_res);

}