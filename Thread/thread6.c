#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine (void* arg) {
	int index = *(int*)arg; // poichè arg è un punt a void, lo castiamo in puntatore ad un puntatore ad un intero
	int sum = 0, i;
	for(i = 0; i < 5; i++) {
		sum = sum + primes[index + i];
	}
	printf("Local sum: %d\n", sum);
	*(int*)arg = sum; // il valore di sum viene messo all'interno dell'indirizzo di arg
	return arg;
}

int main() {
	pthread_t th[2];
	int i;
	for(i = 0; i < 2; i++) {
		int* a = malloc(sizeof(int)); // creo uno spazio dinamico per passare l'argomento al thread
		*a = i*5;
		if(pthread_create(&th[i], NULL, &routine, a) != 0) {
			perror("CREATE");
			exit(EXIT_FAILURE);
		}
	}
	int globalSum = 0;
	for(i = 0; i< 2; i++) {
		int* r; // qui verrà memorizzato il valore che ritorna il thread
		if(pthread_join(th[i], (void**) &r) != 0) {
			perror("JOIN");
			exit(EXIT_FAILURE);
		}
		globalSum = globalSum + *r;
		free(r);
	}
	printf("Global Sum: %d\n", globalSum);
}
