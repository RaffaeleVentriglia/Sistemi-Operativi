#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_THREAD 16

sem_t semaphore;

void* routine(void* arg) {
    printf("(%d) Sono in coda\n", *(int*)arg);
    sem_wait(&semaphore);
    printf("(%d) Login effettuato\n", *(int*)arg);
    sleep(rand() % 5 + 1);
    printf("(%d) Effettuo il logout\n", *(int*)arg);
    sem_post(&semaphore);
    free(arg);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t th[NUM_THREAD];
    sem_init(&semaphore, 0, 5);
    int i;
    for(i = 0; i < NUM_THREAD; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("CREATE");
            exit(EXIT_FAILURE);
        }
    }
    for(i = 0; i < NUM_THREAD; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    sem_destroy(&semaphore);
}