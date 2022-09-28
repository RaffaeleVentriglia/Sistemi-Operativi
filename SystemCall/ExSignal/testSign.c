#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>


static void handler(int signo) {
    printf("Segnale ricevuto: %d\n", signo);
    sleep(1);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pid_t pid;
    int fd[2];
    struct sigaction sa;
    sa.sa_handler = &handler;
    sigaction(SIGUSR1, &sa, NULL);
    if(pipe(fd) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    } else if(pid == 0) {
        // child process
        int n;
        while(n < 10) {
            kill(getppid(), SIGUSR1);
            n++;
        }
    } else {
        // parent process
        int n;
        pause();
        while(n < 10) {
            kill(pid, SIGUSR1);
            n++;
        }
    }
    return 0;
}