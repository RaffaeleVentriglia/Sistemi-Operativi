#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if(pid==0) {
        close(fd[0]);
        int n = 5;
        write(fd[1], &n, sizeof(n));
        close(fd[1]);
    } else {
        int m;
        close(fd[1]);
        read(fd[0], &m, sizeof(m));
        printf("%d\n", m);
    }
}