#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t pid; 
    char com[256];
    int status;
    /*
    pid = fork();
    if(pid == 0) {
        printf("Processo figlio\n");
        execlp("ls", "ls", 0);
    } else {
        wait(&status);
        printf("Processo padre\n");
    }
    */
    while(1) {
        printf("Digitare comando: ");
        scanf("%s", com);
        if(strcmp(com, "exit") == 0) break;
        pid = fork();
        if(pid == -1) {
            printf("Errore nella fork\n");
            exit(1);
        }
        if(pid == 0) {
            execlp(com, com, 0);
        } else {
            wait(&status);
        }
    }
}