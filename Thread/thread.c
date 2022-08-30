#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* routine() {
    printf("Test from threads\n");
    sleep(2);
    printf("Finisce l'esecuzione\n");
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    if(pthread_create(&p1, NULL, &routine, NULL) != 0) {
        perror("PTHREAD_CREATE");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&p2, NULL, &routine, NULL) != 0) {
        perror("PTHREAD_CREATE");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(p1, NULL) != 0) {
        perror("PTHREAD_JOIN");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(p2, NULL) != 0) {
        perror("PTHREAD_JOIN");
        exit(EXIT_FAILURE);
    }
    return 0;
}