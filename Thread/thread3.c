#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N 4

pthread_mutex_t mutex;
int mails = 0;

void* routine() {
    int i;
    for(i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_t th[N];
    int i;
    for(i = 0; i < N; i++) {
        if(pthread_create(th+i, NULL, &routine, NULL) != 0) {
            perror("THREAD");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d sta iniziando\n", i);
    }
    for(i = 0; i < N; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d sta terminando\n", i); // il risultato non è l'ordine effettivo in cui finiscono i thread
    }
    printf("Mail ricevute: %d\n", mails);
    return 0;
}