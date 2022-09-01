#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void* routine() {
	int value = (rand() % 10) + 1;
	int* result = malloc(sizeof(int));
	*result = value;
	printf("Indirizzo del valore nel thread: %p\n", result);
	printf("Valore generato dal thread: %d\n", value);
	return (void *) result;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	pthread_t t1;
	int* res;
	if(pthread_create(&t1, NULL, &routine, NULL) != 0) {
		perror("CREATE");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(t1, (void **) &res) != 0) {
		perror("JOIN");
		exit(EXIT_FAILURE);
	}
	printf("Indirizzo del valore: %p\n", res);
	printf("Valore ricevuto: %d\n", *res);
	free(res);
	return 0;
}
