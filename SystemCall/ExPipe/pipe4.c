#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int p1[2]; // dal figlio al padre
    int p2[2]; // dal padre al figlio
    pid_t pid;
    if(pipe(p1) == -1) { exit(EXIT_FAILURE); }
    if(pipe(p2) == -1) { exit(EXIT_FAILURE); }
    pid = fork();
    if(pid == -1) { exit(EXIT_FAILURE); }
    if(pid == 0) {
        // child process
        close(p1[0]);
        close(p2[1]);
        int x;
        if(read(p2[0], &x, sizeof(x)) == -1) { exit(EXIT_FAILURE); }
        close(p2[1]);
        printf("Valore ricevuto dal processo padre: %d\n", x);
        x *= 4;
        if(write(p1[1], &x, sizeof(x)) == -1) { exit(EXIT_FAILURE); }
        close(p1[0]);
        printf("Valore calcolato inviato al processo padre: %d\n", x);
    } else {
        // parent process
        close(p2[0]);
        close(p1[1]);
        srand(time(NULL));
        int y = rand() % 10;
        int val;
        if(write(p2[1], &y, sizeof(y)) == -1) { exit(EXIT_FAILURE); }
        close(p2[1]);
        printf("Valore inviato al processo figlio: %d\n", y);
        if(read(p1[0], &val, sizeof(val)) == -1) { exit(EXIT_FAILURE); }
        close(p1[1]);
        printf("Valore finale ricevuto dal processo figlio: %d\n", val);
    }
}