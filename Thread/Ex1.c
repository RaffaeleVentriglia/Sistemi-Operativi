#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int m = 10;

void* routine(void* arg) {
    int n = *(int*) arg;
    m += n;
}

int main(int argc, char* argv[]) {
    pthread_t t1;
    int* val = malloc(sizeof(int));
    *val = atoi(argv[1]);
    printf("Valore letto da riga di comando: %d\n", *(int*)val);
    if(pthread_create(&t1, NULL, &routine, val) != 0) {
        perror("CREATE");
        exit(EXIT_FAILURE);
    }
    if(pthread_join(t1, (void**) &val) != 0) {
        perror("JOIN");
        exit(EXIT_FAILURE);
    }
    printf("Valore calcolato: %d\n", val);
    printf("Indirizzo: %p\n", &val);
}