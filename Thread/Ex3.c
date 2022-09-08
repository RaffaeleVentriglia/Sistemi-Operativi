/*
    Facendo uso della libreria Pthread, realizzare un programma in
    cui un thread scrittore, dato un intero N da riga di comando
    (dove 10<N<=15), scrive in un file nella prima posizione, uno alla
    volta ed ogni ½ secondo, la sequenza di Fibonacci di ordine N,
    alternandosi con un thread lettore che legge, uno alla volta dalla
    prima posizione del file, i numeri scritti dal thread scrittore. Un
    terzo thread attende la lettura dell’ N-esimo intero, quindi
    stampa a video il messaggio “Operazioni concluse, arrivederci
    dal thread: tid”, attende 5 secondi e termina.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* scrittore(void* arg) {
	int num = *(int*)arg;
	printf("Thread scrittore\n");
	printf("Numero ricevuto dal thread scrittore: %d\n", num);
}

void* lettore(void* arg) {
	printf("Thread lettore\n");
}

void* termin(void* arg) {
	printf("Thread terminatore\n");
}

int main(int argc, char* argv[]) {
	pthread_t th[3];
	pthread_mutex_init(&mutex, NULL);
	int i, N = atoi(argv[1]);
	int* num = malloc(sizeof(int));
	*num = N;
	printf("Numero inserito da linea di comando: %d\n", N);
	for(i = 0; i < 3; i++) {
		if(i == 1) {
			if(pthread_create(&th[i], NULL, &scrittore, num) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		} else if(i == 2) {
			if(pthread_create(&th[i], NULL, &lettore, NULL) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		} else {
			if(pthread_create(&th[i], NULL, &termin, NULL) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		}
	}
	for(i = 0; i < 3; i++) {
		if(pthread_join(th[i], NULL) != 0) {
			perror("JOIN");
			exit(EXIT_FAILURE);
		}
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
