#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void* routine() {
    for(int i = 0; i < 100000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    if(pthread_create(&t1, NULL, &routine, NULL) != 0) {
        perror("PTHREAD_CREATE");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&t2, NULL, &routine, NULL) != 0) {
        perror("PTHREAD_CREATE");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(t1, NULL) != 0) {
        perror("PTHREAD_JOIN");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(t2, NULL) != 0) {
        perror("PTHREAD_JOIN");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_destroy(&mutex);
    printf("Numero di mail: %d\n", mails);
}