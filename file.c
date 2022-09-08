#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// HEADER PER LE OPTION FLAGS
#include <fcntl.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {
    int ds_file, ds_file2, ds_file3;
    char buffer[BUFSIZE];
    // CREAZIONE DI UN FILE
    if (creat("Prova", 0666) == -1) {
        printf("Errore nella chiamata creat\n");
        exit(1);
    }
    // APERTURA DI UN FILE
    ds_file = open("Pippo", O_RDONLY);
    ds_file2 = open("Prova", O_RDWR);
    lseek(ds_file, 0, SEEK_SET); // CAMBIARE LA POSIZIONE DEL PUNTATORE NEL FILE
    // APERTURA DI UN FILE CON NOME DA RIGA DI COMANDO: ./file Pippo
    ds_file3 = open(argv[1], O_RDWR);
    if (ds_file == -1) {
        printf("Errore nell'apertura del file\n");
    } else {
        // LETTURA DI UN FILE
        read(ds_file, buffer, BUFSIZE);
        // IMPOSTARE L'ULTIMO ELEMENTO (inutile)
        //buffer[BUFSIZE] = '\0';
        printf("Contenuto del file: \n%s\n", buffer);
        // SCRITTURA IN UN FILE
        if (write(ds_file2, buffer, BUFSIZE) == -1) {
            printf("Errore in scrittura\n");
        }
        // CHIUSURA DI UN FILE
        close(ds_file);
        close(ds_file2);
        // UTILIZZO DEI LINK E UNLINK
        if(link(argv[1], argv[2]) == -1) {
            printf("Error\n");
            exit(1);
        }
        if(unlink(argv[1]) == -1) {
            printf("Error\n");
            exit(1);
        }
    }
}