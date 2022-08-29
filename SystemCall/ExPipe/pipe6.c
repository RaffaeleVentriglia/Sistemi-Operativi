#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if(pipe(fd) == -1) {
        perror("PIPE");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();
    if(pid == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        // child process
        close(fd[0]);
        char str[256];
        int length;
        printf("Inserire una stringa: ");
        fgets(str, 256, stdin);
        str[strlen(str) - 1] = '\0'; // ultimo carattere \0
        length = strlen(str) + 1;
        printf("Lunghezza stringa: %d\n", length);
        if(write(fd[1], &length, sizeof(int)) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
        printf("Invio stringa\n");
        if(write(fd[1], str, length) == -1) {
            perror("WRITE");
            exit(EXIT_FAILURE);
        }
        printf("Stringa inviata\n");
        close(fd[1]);
    } else {
        // parent process
        close(fd[1]);
        char str[256];
        int length;
        if(read(fd[0], &length, sizeof(int)) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        printf("Lunghezza della stringa che riceverò: %d\n", length);
        if(read(fd[0], str, length) == -1) {
            perror("READ");
            exit(EXIT_FAILURE);
        }
        printf("Stringa ricevuta: %s\n", str);
        close(fd[0]);
        wait(NULL);
    }
}