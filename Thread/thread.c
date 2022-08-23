#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tid1;
pthread_t tid2;

void printid();
void *thr_fn();
void *thr_fn1();
void *thr_fn2();

int main(int argc, char *argv[]) {
    pthread_mutex_t prova; // creo un mutex
    int err;
    void *tret;

    err = pthread_mutex_init(&prova, NULL); // inizializzazione di un thread
    err = pthread_create(&tid1, NULL, thr_fn1, NULL); // creazione di un thread
    if(err != 0) {
        printf("Non sono riuscito a creare il thread 1\n");
        exit(EXIT_FAILURE);
    }
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if(err != 0) {
        printf("Non sono riuscito a creare il thread 2\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_cancel(tid2) == 0) { // cancella il thread
        printf("Thread 2 cancellato\n");
    }
    err = pthread_join(tid1, &tret); //join di un thread
    if(err != 0) {
        printf("Non sono riuscito a fare il join\n");
    } printf("Exit code thread 1: %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if(err != 0) {
        printf("Non sono riuscito a fare il join\n");
    } printf("Exit code thread 2: %ld\n", (long)tret);
    //printid();
    sleep(1);
    return 0;
}

void printid() {
    pid_t pid;
    pthread_t th1;
    pid = getpid();
    th1 = pthread_self();
    printf("%ld -- %d\n", th1, pid);
}

void *thr_fn() {
    printid();
    return((void *)0);
}

void *thr_fn1() {
    printf("Thread 1 che ritorna\n");
    return((void *)1);
}

void *thr_fn2() {
    printf("Thread 2 che esce\n");
    pthread_exit((void *)2);
}