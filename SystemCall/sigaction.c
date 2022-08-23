#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void signal_handler(int);

int main(int argc, char *argv[]) {
    int x;
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = &signal_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    printf("In attesa di un segnale\n");
    while(1) pause();

    exit(EXIT_SUCCESS);
}

void signal_handler(int signo) {
        printf("\nCodice segnale ricevuto: %d\n", signo);
}


/*
    Nel momento in cui creo un handler (ricevitore) di un determinato segnale,
    cambio il modo di agire del processo nel momento in cui riceve questo tipo di segnale.
*/