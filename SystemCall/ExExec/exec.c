#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {

    pid_t childpid = 0;
    int status = 0;

    if((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(childpid == 0) {
        printf("Sono il processo figlio %d\n", getpid());
        execl("/bin/ls", argv[1], argv[2], argv[argc]);
    } puts("");

    if(wait(&status) < 0) {
        perror("WAIT");
        exit(EXIT_FAILURE);
    }



    if((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(childpid == 0) {
        printf("Sono il processo figlio %d\n", getpid());
        execv("/bin/ls", argv+1);
    } puts("");

    if(wait(&status) < 0) {
        perror("WAIT");
        exit(EXIT_FAILURE);
    }



    if((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(childpid == 0) {
        printf("Sono il processo figlio %d\n", getpid());
        execlp(argv[1], argv[1], argv[2], argv[argc]);
    } puts("");

    if(wait(&status) < 0) {
        perror("WAIT");
        exit(EXIT_FAILURE);
    }



    if((childpid = fork()) < 0) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if(childpid == 0) {
        printf("Sono il processo figlio %d\n", getpid());
        execvp(argv[1], argv+1);
    } puts("");

    if(wait(&status) < 0) {
        perror("WAIT");
        exit(EXIT_FAILURE);
    }

    printf("Sono il processo padre %d\n", getpid());
    printf("Termino l'esecuzione del programma\n");
    exit(EXIT_SUCCESS);

}