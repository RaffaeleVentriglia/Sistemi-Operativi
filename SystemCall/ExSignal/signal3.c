#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

int x;

void handler_sigusr1(int signo) {
    printf("Valore inviato\n");
}

void handler_sigusr2(int signo) {
    printf("Valore letto, %d\n", x);
}

int main(int argc, char *argv[]) {
    struct sigaction sa2;
    sa2.sa_handler = &handler_sigusr1;
    sa2.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa2, NULL);

    struct sigaction sa;
    sa.sa_handler = &handler_sigusr2;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &sa, NULL);

    pid_t pid = fork();
    if(pid == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        srand(time(NULL));
        x = rand() % 10;
        printf("Valore generato: %d\n", x);
        kill(getppid(), SIGUSR1);
        sleep(2);
    } else {
        sleep(5);
        kill(pid, SIGUSR2);
    }
    return 0;
}