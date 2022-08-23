#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    pid_t pid; 
    char com[256];
    int status;

    // pid = fork();
    // if(pid == 0) {
    //     printf("Processo figlio\n");
    //     execlp("ls", "ls", 0);
    // } else {
    //     wait(&status);
    //     printf("Processo padre\n");
    // }

   // ESEGUIRE UNA LINEA DI COMANDO
    // while(1) {
    //     printf("Digitare comando: ");
    //     scanf("%s", com);
    //     if(strcmp(com, "exit") == 0) break;
    //     pid = fork();
    //     if(pid == -1) {
    //         printf("Errore nella fork\n");
    //         exit(1);
    //     }
    //     if(pid == 0) {
    //         execlp(com, com, 0);
    //     } else {
    //         wait(&status);
    //     }
    // }

    int id = fork();
    int id2 = fork();
    if(id == 0) {
        if(id2 == 0) {
            printf("Processo y\n");
        } else {
            printf("Processo x\n");
        }
    } else {
        if(id2 == 0) {
            printf("Processo z\n");
        } else {
            printf("Processo padre\n");
        }
    }
    while(wait(NULL) != -1 || errno != ECHILD) {
        printf("Aspettando che un processo figlio finisca\n");
    }
    wait(NULL);

    // if(id == 0) {
    //     sleep(1);
    // }
    // printf("Current ID: %d, parent ID: %d\n", getpid(), getppid());
    // int res = wait(NULL);
    // if(res == -1) {
    //     printf("Non ci sono processi figli da aspettare\n");
    // } else {
    //     printf("%d\n", res);
    // }

    // int n, i;
    // if(id == 0) {
    //     n = 1;
    // } else {
    //     n = 6;
    // }
    // // aspetta che il processo figlio termini l'esecuzione
    // wait(&status);
    // for(i = n; i < n + 5; i++) {
    //     printf("%d ", i);
    //     fflush(stdout);
    // }
    // if(id != 0) {
    //     printf("\n");
    // }
    // return 0;
}