#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int x = 99;
    int fd;
    // CREO UNA FIFO
    if(mkfifo("prova1", 0777) == -1) {
        if(errno != EEXIST) {
            perror("FIFO");
            exit(EXIT_FAILURE);
        }
    }
    if((pid = fork()) == 0) {
        // APRO LA FIFO
        fd = open("prova1", O_WRONLY); // open ritorna un file descriptor
        // se avessi aperto la fifo con modalità lettura - scrittura (O_RDWR)
        // allora non avrei avuto bisogno di un processo che leggesse il contenuto
        // in quanto se non c'è nessuno che legge la fifo non potrà continuare l'esecuzione

        // SCRIVO SULLA FIFO
        if(write(fd, &x, sizeof(x)) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
        close(fd);
    } else {
        int y;
        // LEGGO LA FIFO
        fd = open("prova1", O_RDONLY);
        // LEGGO LA FIFO
        if(read(fd, &y, sizeof(y)) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        close(fd);
        printf("Valore ottenuto dalla FIFO: %d\n", y);
    }
    return 0;
}