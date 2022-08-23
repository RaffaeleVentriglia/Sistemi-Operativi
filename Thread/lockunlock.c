#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct pr {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};
typedef struct pr struttura;

struttura *pr_alloc(int id) {
    struttura *prova;
    if((prova = malloc(sizeof(struct pr))) != NULL) {
        prova->f_count = 1;
        prova->f_id = id;
        if(pthread_mutex_init(&prova->f_lock, NULL) != 0) {
            free(prova);
            return(NULL);
        }
    }
    return(prova);
}

void pr_hold(struttura *prova) {
    pthread_mutex_lock(&prova->f_lock);
    prova->f_count++;
    pthread_mutex_unlock(&prova->f_lock);
}

void pr_rele(struttura *prova) {
    pthread_mutex_lock(&prova->f_lock);
    if(--prova->f_count == 0) {
        pthread_mutex_unlock(&prova->f_lock);
        pthread_mutex_destroy(&prova->f_lock);
        free(prova);
    } else {
        pthread_mutex_unlock(&prova->f_lock);
    }
}