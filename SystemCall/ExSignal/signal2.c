#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler_signal(int signo) {
    printf("\nSTATT FERM\n");
}

void handler_sigcont(int signo) {
    printf("Inserire un numero: ");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    sa.sa_handler = &handler_sigcont;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCONT, &sa, NULL);
    //signal(SIGTSTP, &handler_signal);
    int x;
    printf("Inserire un numero: ");
    scanf("%d", &x);
    printf("Risultato di %d * 5: %d\n", x, x*5);
    return 0;
}