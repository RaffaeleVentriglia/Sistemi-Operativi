#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex[4];
int stoveFuel[4] = {100, 100, 100, 100};

void* routine(void* arg) {
    int i;
    for(i = 0; i < 4; i++) {
        if(pthread_mutex_trylock(&mutex[i]) == 0) {
            int fuelNedeed = (rand() % 20) + 1;
            if(stoveFuel[i]-fuelNedeed < 0) {
                printf("Non c'è abbastanza carburante\n");
            } else {
                stoveFuel[i] -= fuelNedeed;
                usleep(500000);
                printf("Carburante rimanente: %d\n", stoveFuel[i]);
            }
            pthread_mutex_unlock(&mutex[i]);
            break;
        } else {
            if(i == 3) {
                printf("Fornelli non disponibili, aspetto...\n");
                usleep(300000);
                i = 0;
            }
        }
    } 
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t th[10];
    int i;
    for(i = 0; i < 4; i++) {
        pthread_mutex_init(&mutex[i], NULL);
    }
    for(i = 0; i < 10; i++) {
        if(pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("CREATE");
            exit(EXIT_FAILURE);
        }
    }
    for(i = 0; i < 10; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    for(i = 0; i < 4; i++) {
        pthread_mutex_destroy(&mutex[i]);
    }
}