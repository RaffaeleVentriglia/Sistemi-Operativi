#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t pid;
    if((pid = fork()) == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        printf("Sono il processo %d con padre %d\n", getpid(), getppid());
        int err = execlp("ping", "ping", "-c", "1", "google.com", NULL);
        if(err == -1) {
            printf("Non è stato possibile trovare il comando\n");
            return 1;
        }
    } else {
        int status;
        wait(&status);
        if(WIFEXITED(status)) {
            int statusCode = WEXITSTATUS(status);
            if(statusCode == 0) {
                printf("Exec eseguita con successo!\n");
            } else {
                printf("Fallimento con status code: %d\n", statusCode);
            }
        }
        
    }
    printf("Sono il processo %d\n", getpid());
    return 0;
}