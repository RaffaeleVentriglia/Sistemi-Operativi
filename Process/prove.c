#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t childpid, fatherpid;
    fatherpid = getpid();
    if((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    printf("Ciao dal processo: %d\n", getpid());
    return 0;
}