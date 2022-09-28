/*
    Si  realizzi  un  programma  C  e  Posix  in  ambiente  Linux  che,  impiegando  la  libreria  Pthread,  generi  tre  thread.
    I primi due thread sommano 1000 numeri generati casualmente ed ogni volta incrementano un  contatore.
    Il terzo thread attende che il contatore incrementato dai due thread raggiunga un valore limite fornito da riga di comando,
    notifica l’avvenuta condizione e termina. Utilizzare le variabili condizione.
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define THREAD 3

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t th[THREAD];

int cont = 0;
int somma = 0;

void* routine(void* arg) {
	int limite = *(int*)arg;
	printf("Thread routine creato\n");
	while(cont < limite) {
		int num = rand()%3+1;
		pthread_mutex_lock(&mutex);
		somma += num;
		cont++;
		printf("Somma: %d\n", somma);
		printf("Contatore: %d\n", cont);
		if(cont == limite) {
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&cond);
			pthread_exit(0);
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(0);
}

void* ultimo(void* arg) {
	printf("Thread ultimo creato\n");
	int limite = *(int*)arg;
	if(cont < limite) {
		pthread_cond_wait(&cond, &mutex);
		printf("Il contatore ha raggiunto il limite\n");
		pthread_exit(0);
	}	
	pthread_exit(0);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	int limite = atoi(argv[1]);

	for(int i = 0; i < THREAD; i++) {
		if(i < 2) {
			int *x = malloc(sizeof(int));
			*x = limite;
			if(pthread_create(&th[i], NULL, &routine, x) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		} else {
			int *x = malloc(sizeof(int));
			*x = limite;
			if(pthread_create(&th[i], NULL, &ultimo, x) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		}
	}

	for(int i = 0; i < THREAD; i++) {
		if(pthread_join(th[i], NULL) != 0) {
			perror("JOIN");
			exit(EXIT_FAILURE);
		}
	}

	printf("Somma totale: %d\n", somma);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
