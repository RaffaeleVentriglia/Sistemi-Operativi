#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    execlp("ping", "ping", "-c", "1", "google.com", NULL);
    printf("Esecuzione avvenuta\n");
    return 0;
}