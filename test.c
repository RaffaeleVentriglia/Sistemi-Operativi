#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    for(i = 1; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    int n = atoi(argv[1]);
    printf("%d\n", n);
    exit(0);
}