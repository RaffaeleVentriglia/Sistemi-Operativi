#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    pid_t pid = 0;
    for(i = 1; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    if(!pid) {
        fork();
    }

    int n = atoi(argv[1]);
    printf("%d\n", n);
    exit(0);
}