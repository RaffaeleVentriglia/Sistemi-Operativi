#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

int fuel = 0;
pthread_cond_t condFuel;
pthread_mutex_t mutexFuel;

void* fuel_fill(void* arg) {
    int i;
    for(i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        printf("POMPA: Sto ricaricando la benzina... ");
        fuel += 60;
        printf("Quantità di carburante: %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        //pthread_cond_signal(&condFuel); INVIA IL SEGNALE AD UN SINGOLO THREAD IN ATTESA
        pthread_cond_broadcast(&condFuel); // INVIA IL SEGNALE A TUTTI I THREAD IN ATTESA
        sleep(1);
    }
}

void* car(void* arg) {
    pthread_mutex_lock(&mutexFuel);
    while(fuel < 40) {
        printf("MACCHINA: Carburante presente non sufficiente...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
    }
    printf("MACCHINA: Sto facendo rifornimento... ");
    fuel -= 40;
    printf("Rifornimento avvenuto. Carburante rimanente: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[]) {
    pthread_t th[5];
    pthread_cond_init(&condFuel, NULL);
    pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for(i = 0; i < 5; i++) {
        if(i == 4) {
            if(pthread_create(&th[i], NULL, &fuel_fill, NULL) != 0) {
                perror("CREATE");
                exit(EXIT_FAILURE);
            }
        } else {
            if(pthread_create(&th[i], NULL, &car, NULL) != 0) {
                perror("CREATE");
                exit(EXIT_FAILURE);
            }
        }
    }
    for(i = 0; i < 5; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}