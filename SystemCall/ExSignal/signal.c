#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

static void signal_handler(int);

int main(int argc, char *argv[]) {
    pid_t childpid = 0;
    int status = 0;
    signal(SIGINT, signal_handler);
    if ((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    wait(&status);
    if (childpid == 0) {
        sleep(5);
        printf("Processo figlio\n");
        while(1)
            sleep(1);
    }
}

static void signal_handler(int signo) {
    if(signo == SIGINT) printf("SIGNAL RECEIVED: %d\n",signo);
}