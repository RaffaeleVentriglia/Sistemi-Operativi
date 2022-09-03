#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* routine(void* arg) {
    int value = (rand()%6)+1;
    int* result = malloc(sizeof(int));
    *result = value;
    // return (void*) result;
    pthread_exit((void*) result);
}

int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;
    if(pthread_create(&th, NULL, &routine, NULL) != 0) {
        perror("ERROR 1");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(th, (void**) &res) != 0) {
        perror("ERROR 2");
        exit(EXIT_FAILURE);
    }
    printf("Risultato: %d\n", *res);
    free(res);
}