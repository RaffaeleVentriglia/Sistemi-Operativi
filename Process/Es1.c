/*

    Scrivere un programma in C e Posix sotto Linux che, preso un argomento
    intero positivo da riga di comando, gestisca la seguente situazione:
    genera due figli A e B e
        - se l'argomento è PARI invia un segnale SIGUSR1 alla ricezione del quale il
        figlio A calcola il cubo del numero passato come argomento da linea di
        comando, mentre il figlio B stampa un messaggio di arrivederci e
        termina.
        - se l'argomento è DISPARI invia un segnale SIGUSR2 alla ricezione del
        quale il figlio B calcola il reciproco del numero passato come argomento,
        attende per un numero di secondi pari al doppio del numero passato
        come argomento ed invia un segnale SIGUSR1 al processo A dopodiché
        termina l'esecuzione. Il figlio A, invece, attende la ricezione del segnale
        SIGUSR1, stampa un messaggio e termina.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

pid_t pidA, pidB;
int segnale, intero;

void gestore(int);

int main (int argc, char *argv[]) {
    int cubo_int;
    float rec_int;
    signal(SIGUSR1, gestore);
    signal(SIGUSR2, gestore);
    intero = atoi(argv[1]);

    // Processo figlio A
    pidA = fork();
    if (pidA == 0) {
        pause();
        if (segnale == SIGUSR1) { // segnale sigusr1 inviato dal padre
            cubo_int = (intero*intero*intero);
            printf("Figlio A: cubo dell'intero è: %d\n", cubo_int);
            exit(0);
        } else {
            printf("Figlio A: intero DISPARI, attendo un segnale da B\n");
            pause();
            if (segnale == SIGUSR1) { // segnale siusr1 inviato da B
                printf("Figlio A: segnale arrivato, termino\n");
                exit(0);
            }
        }
    }

    pidB = fork();
    if (pidB == 0) {
        pause();
        if (segnale == SIGUSR1) {
            printf("Figlio B: intero pari, arrivederci\n");
            exit(0);
        } else {
            rec_int = (float)1/intero;
            printf("Figlio B: reciproco intero ricevuto: %f\n", rec_int);
            sleep(2*intero);
            kill(pidA, SIGUSR1);
            exit(0);
        }
    }

    if ((pidA && pidB) != 0) {
        sleep(1);
        if (intero % 2 == 0) {
            kill(pidA, SIGUSR1);
            kill(pidB, SIGUSR1);
        } else {
            kill(pidA, SIGUSR2);
            kill(pidB, SIGUSR2);
        }
    }
    wait(NULL);
    wait(NULL);
    exit(0);
}

// creo un gestore di segnali
void gestore (int sign) {
    segnale = sign;
}