#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* routine(void* arg) {
    if(pthread_mutex_trylock(&mutex) == 0) {
        printf("Locked\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Non posso eseguire il lock\n");
    }
}

int main(int argc, char *argv[]) {
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    int i;
    for(i = 0; i < 4; i++) {
        if(pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("CREATE");
            exit(EXIT_FAILURE);
        }
    }
    for(i = 0; i < 4; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_destroy(&mutex);
}