#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_THREADS 4

pthread_mutex_t mutex;
sem_t semEmpty;
sem_t semFull;

int buffer[10];
int count = 0;

void* producer(void* arg) {
    while(1) {
        int x = rand() % 100;
        sleep(1);
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutex);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&semFull);
    }
}

void* consumer(void* arg) {
    while(1) {
        int y;
        sem_wait(&semFull);
        pthread_mutex_lock(&mutex);
        y = buffer[count-1];
        count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&semEmpty);
        printf("Valore: %d\n", y);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    pthread_t th[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    int i;
    for(i = 0; i < NUM_THREADS; i++) {
        if(i % 2 == 0) {
            if(pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("CREATE 1");
                exit(EXIT_FAILURE);
            }
        } else {
            if(pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("CREATE 2");
                exit(EXIT_FAILURE);
            }
        }
    }
    for(i = 0; i < NUM_THREADS; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    sem_destroy(&semFull);
    sem_destroy(&semEmpty);
    pthread_mutex_destroy(&mutex);
    return 0;
}