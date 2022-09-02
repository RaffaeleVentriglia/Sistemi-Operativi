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

int varGlob;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* fatt(void* arg) {
    int N = *(int*)arg;
    printf("N ricevuto: %d\n", N);
    int i, num, fatt = 0;
    for(i = 0; i < N; i++) {
        num = (rand() % 10) + 1;
        printf("Numero generato: %d\n", num);
        if(num > 0) {
            while(num > 0) {
                fatt = fatt * num;
                num--;
            }
        }
        pthread_mutex_lock(&mutex);
        varGlob = fatt;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
}

void* stampa(void* arg) {
    sleep(1);
    int N = *(int*)arg;
    int i;
    for(i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        while(varGlob == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Fattoriale n.%d: %d\n", i, varGlob);
        pthread_mutex_unlock(&mutex);
    }
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t th[2];
    int i;
    int* N = malloc(sizeof(int));
    *N = atoi(argv[1]);
    for(i = 0; i < 2; i++) {
        if(i == 1) {
            if(pthread_create(&th[i], NULL, &stampa, N) != 0) {
                perror("CREATE");
                exit(EXIT_FAILURE);
            }
        } else {
            if(pthread_create(&th[i], NULL, &fatt, N) != 0) {
                perror("CREATE");
                exit(EXIT_FAILURE);
            }
        }
    }
    for(i = 0; i < 2; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}