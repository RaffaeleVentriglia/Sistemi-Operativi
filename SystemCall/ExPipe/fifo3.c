#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    if(mkfifo("prova", 0777) == -1) {
        if(errno != EEXIST) {
            perror("FIFO");
            exit(EXIT_FAILURE);
        }
    }
    int fd = open("prova", O_RDWR);
    pid_t pid = fork();
    if(pid == 0) {
        // child process
        int x;
        read(fd, &x, sizeof(x));
        printf("Valore ricevuto dal padre tramite FIFO: %d\n", x);
        x *= 4;
        write(fd, &x, sizeof(x));
        printf("Valore calcolato inviato al padre: %d\n", x);
    } else {
        // parent process
        srand(time(NULL));
        int y = rand() % 10;
        write(fd, &y, sizeof(y));
        printf("Valore scritto sulla FIFO: %d\n", y);
        read(fd, &y, sizeof(y));
        printf("Valore ricevuto dal figlio tramite FIFO: %d\n", y);
    }
}