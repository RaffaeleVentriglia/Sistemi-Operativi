/*
    Si realizzi un programma C e posix in ambiente Linux che, impiegando la libreria pthread,
    generi m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm. 
    L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
    inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione
    libera disponibile. Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa,
    provvederà a stamparne il contenuto. Le matrici devono essere allocate dinamicamente.
    Usare mutex e variabili di condizione.
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define m 4

pthread_mutex_t mutex;
pthread_cond_t cond;

int *a;
int *b;
int *c;
int j;

void creaArray(int *a) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            *(a+i*m+j) = rand()%5+1;
        }
    }
}

void stampaArray(int *a) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            printf("%2u", *(a+i*m+j));
        }
        puts("");
    }
}

int spazio(){
    for(int i=0; i<m; i++)
        if(*(c+i) == 0)
            return i;
    return m;
}

void prodotto(int index){
    int tempA = 1;
    int tempB = 1;
    int tempC = 0;
    for(int i=0; i<m; i++)
        tempA = (*(a+i*m+index)) * tempA;
    for(int i=0; i<m; i++)
        tempB = (*(b+i*m+index)) * tempB;
    tempC = tempA + tempB;
    *(c+j) = tempC;
}

// routine m
void* routine(void *ind){
    int index = *(int*)ind;
    // sezione critica
    pthread_mutex_lock(&mutex);
    j = spazio();
    if(j == (m-1)){
        prodotto(index);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        pthread_exit(0);       
    } else 
        prodotto(index);
    // sezione critica
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

// routine finale
void* lettore(){
    pthread_mutex_lock(&mutex);
    while (j != (m-1))
        pthread_cond_wait(&cond, &mutex);
    printf("Vettore c completo\n");
    for (int i=0; i<m; i++)
        printf("%2u ", *(c+i));
    printf("\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}


int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    srand(time(NULL));
    a = malloc(m*m*sizeof(int*));
    b = malloc(m*m*sizeof(int*));
    c = malloc(m*sizeof(int*));

    creaArray(a);
    creaArray(b);

    printf("Array a\n");
    stampaArray(a);
    printf("Arry b\n");
    stampaArray(b);

    // creo e visualizzo il vettore c
    for(int i = 0; i < m; i++) {
        *(c+i) = 0;
    }
    printf("Vettore c\n");
    for(int i = 0; i < m; i++) {
        printf("%2u ", *(c+i));
    }
    puts("");

    pthread_t th[m+1];
    for(int i = 0; i < m; i++) {
        int *index = malloc(sizeof(int));
        *index = i;
        if(pthread_create(th+i, NULL, &routine, index) != 0) {
            perror("CREATE");
            exit(EXIT_FAILURE);
        }
    }

    if(pthread_create(th+m, NULL, &lettore, NULL) != 0) {
        perror("CREATE");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i <= m; i++) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("JOIN");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    free(a);
    free(b);
    free(c);
    
}