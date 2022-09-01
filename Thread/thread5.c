#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void* routine(void* arg) {
	int index = *(int*)arg;
	printf("%d ", primes[index]);
	free(arg);
}

int main() {
	pthread_t th[10];
	int i;
	for(i = 0; i < 10; i++) {
		int* a = malloc(sizeof(int));
		*a = i;
		if(pthread_create(&th[i], NULL, &routine, a) != 0) {
			perror("CREATE");
			exit(EXIT_FAILURE);
		}
	}
	for(i = 0; i < 10; i++) {
		if(pthread_join(th[i], NULL) != 0) {
			perror("JOIN");
			exit(EXIT_FAILURE);
		}
	}
	printf("\n");
}
