#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if(pid == -1) {
        perror("FORK");
        exit(EXIT_FAILURE);
    }
    if(pid == 0) {
        while(1) {
            printf("Ciao\n");
            sleep(1);
        }
    } else {
        kill(pid, SIGSTOP);
        int t;
        do {
            printf("Inserire secondi: ");
            scanf("%d", &t);
            if(t > 0) {
                kill(pid, SIGCONT);
                sleep(t);
                kill(pid, SIGSTOP);
            }
        } while(t > 0);
        kill(pid, SIGKILL);
        wait(NULL);
    }
}