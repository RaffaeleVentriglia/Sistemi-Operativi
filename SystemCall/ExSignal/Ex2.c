/*
    un processo padre P0 deve creare 2 processi figli: P1 e P2;
        • il figlio P1 deve aspettare N1 secondi e successivamente eseguire
        il comando C
        • il figlio P2 dopo N2 secondi dalla sua creazione dovrà provocare la
        terminazione del processo fratello P1 e successivamente
        terminare
        • nel frattempo, P2 deve periodicamente sincronizzarsi con il padre
        P0 (si assuma la frequenza di 1 segnale al secondo)
        • il padre P0, dopo aver creato i figli, si pone in attesa di segnali da
        P2: per ogni segnale ricevuto, dovrà stampare il proprio pid; all’
        N-esimo segnale ricevuto dovrà attendere la terminazione dei
        figli e successivamente terminare
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int sig;
pid_t p1, p2;

static void handler(int signo) {
    sig = signo;
}

int main(int argc, char* argv[]) {
    struct sigaction sa;
    sa.sa_handler = &handler;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGKILL, &sa, NULL);

    int i;
    int N1, N2;
    N1 = atoi(argv[1]);
    N2 = atoi(argv[2]);

    printf("N1 ricevuto: %d\n", N1);
    printf("N2 ricevuto: %d\n", N2);

    // Figlio p1
    p1 = fork();
    if(p1 < 0) {
        perror("FORK 1");
        exit(EXIT_FAILURE);
    } else if(p1 == 0) {
        // Child process
        sleep(N1);
        execlp(argv[3], argv[3], NULL);
        pause();
    }

    // Figlio p2
    p2 = fork();
    if(p2 < 0) {
        perror("FORK 2");
        exit(EXIT_FAILURE);
    } else if(p2 == 0) {
        // Child process
        sleep(N2);
        kill(p1, SIGKILL);
        for(i = 0; i < N2; i++) {
            kill(getppid(), SIGUSR1);
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }

    // Main process
    if((p1 && p2) != 0) {
        for(i = 0; i < N2; i++) {
            pause();
            printf("Il mio pid è: %d\n", getpid());
        }
        wait(NULL);
        wait(NULL);
    }
    return 0;
}