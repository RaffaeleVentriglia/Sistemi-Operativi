#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    pid_t pid1, pid2;
    /*for(i = 1; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }*/

    pid1 = fork();
    pid2 = fork();

    int n = atoi(argv[1]);
    printf("%d\n", n);
    exit(0);
}