/* 
    Scrivere un programma in C e Posix sotto Linux che, preso un argomento intero positivo da riga di comando, gestisca la 
    seguente situazione:
    genera due figli A e B e
        - se l'argomento è PARI invia un segnale SIGUSR1 alla ricezione del quale il figlio A calcola il cubo del numero 
        passato come argomento da linea di comando, mentre il figlio B stampa un messaggio di arrivederci e termina.
        - se l'argomento è DISPARI invia un segnale SIGUSR2 alla ricezione del quale il figlio B calcola il reciproco del 
        numero passato come argomento, attende per un numero di secondi pari al doppio del numero passato come argomento ed 
        invia un segnale SIGUSR1 al processo A dopodiché termina l'esecuzione. Il figlio A, invece, attende la ricezione 
        del segnale SIGUSR1, stampa un messaggio e termina.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int sig;

static void handler(int signo) {
	sig = signo;
}

int main(int argc, char* argv[]) {
	int cubo;
	float reciproco;
	struct sigaction sa;
	sa.sa_handler = &handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);

	pid_t a, b;
	int num = atoi(argv[1]);
	printf("Numero ricevuto da linea di comando: %d\n", num);

	// Figlio A
	a = fork();
	if(a < 0) {
		perror("FORK 1");
		exit(EXIT_FAILURE);
	} else if(a == 0) {
		// child process
		pause();
		if(sig == SIGUSR1) {
			cubo = num*num*num;
			printf("A: Cubo calcolato dal figlio A: %d\n", cubo);
			exit(EXIT_SUCCESS);
		} else {
			printf("A: Attendo segnale SIGUSR1 da B...\n");
			pause();
			if(sig == SIGUSR1) {
				printf("A: Segnale ricevuto, arrivederci\n");
				exit(EXIT_SUCCESS);
			}
		}
	}

	// Figlio B
	b = fork();
	if(b < 0) {
		perror("FORK 2");
		exit(EXIT_FAILURE);
	} else if(b == 0) {
		// child process
		pause();
		if(sig == SIGUSR1) {
			printf("B: Termino l'esecuzione, arrivederci\n");
                        exit(EXIT_SUCCESS);
		} else {
			reciproco = (float) 1/num;
                        printf("B: Reciproco calcolato dal figlio B: %f\n", reciproco);
                        sleep(num*2);
                        kill(a, SIGUSR1);
                        exit(EXIT_SUCCESS);
		}
	}

	// Main process
	if((a && b) != 0) {
		sleep(1);
		if(num % 2 == 0) {
			kill(a, SIGUSR1);
			kill(b, SIGUSR1);
			return 0;
		} else {
			kill(a, SIGUSR2);
			kill(b, SIGUSR2);
			return 0;
		}
		wait(NULL);
		wait(NULL);
	}
	return 0;
}
