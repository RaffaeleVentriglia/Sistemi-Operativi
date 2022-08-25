#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int arr[] = {1, 2, 4, 8, 16, 32};
    int size = sizeof(arr) / sizeof(int);
    int inizio, fine;
    int fd;

    if(mkfifo("Es", 0777) == -1) {
        if(errno != EEXIST) {
            perror("FIFO");
            exit(EXIT_FAILURE);
        }
    }

    pid = fork();
    if(pid == 0) {
        inizio = 0;
        fine = size / 2;
    } else {
        inizio = size / 2;
        fine = size;
    }

    int i, somma = 0;
    for(i = inizio; i < fine; i++) {
        somma = somma + arr[i];
    }
    printf("Somma parziale calcolata: %d\n", somma);

    if(pid == 0) {
        fd = open("Es", O_WRONLY);
        write(fd, &somma, sizeof(somma));
        close(fd);
    } else {
        int sommaParz, sommaTot;
        fd = open("Es", O_RDONLY);
        read(fd, &sommaParz, sizeof(sommaParz));
        close(fd);
        sommaTot = somma + sommaParz;
        printf("Somma totale calcolata: %d\n", sommaTot);
        wait(NULL);
    }
}