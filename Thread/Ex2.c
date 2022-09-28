/*
    Si progetti un programma in ambiente Linux che, impiegando  la  libreria  pthread,  
    crei  un  thread  che  estrae N volte (con N passato da riga di comando) un numero 
    casuale compreso tra 0 e 10 e ne calcoli il fattoriale  ponendolo  in  una  variabile  
    condivisa  e  un  thread  che  stampi  a  video  il  fattoriale  calcolato  N  volte 
    prelevandolo dalla variabile condivisa.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;

void* fatt(void* arg) {
	int N = *(int*)arg;
	int fattoriale;
	printf("Fattoriale\n");
	for(int i = 0; i < N; i++) {
		int x = rand()%(10-1)+1;
		if(x > 0) {
			while(x > 0) {
				fattoriale = x*x-1;
				x--;
			}
		}
		printf("Fattoriale calcolato: %d\n", fattoriale);
	}
}

void* stampa(void* arg) {
	printf("Stampa\n");
}


int main(int argc, char* argv[]) {
	srand(time(NULL));
	pthread_t th[2];

	int num = atoi(argv[1]);

	for(int i = 0; i < 2; i++) {
		if(i == 1) {
			int *N = malloc(sizeof(int));
			*N = num;
			if(pthread_create(&th[i], NULL, &fatt, N) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		} else {
			if(pthread_create(&th[i], NULL, &stampa, NULL) != 0) {
				perror("CREATE");
				exit(EXIT_FAILURE);
			}
		}
	}

	for(int i = 0; i < 2; i++) {
		if(pthread_join(th[i], NULL) != 0) {
			perror("JOIN");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
