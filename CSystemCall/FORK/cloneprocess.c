#include <stdio.h>
#include <unistd.h>

int main() {

 int childpid;
 childpid = fork();
 
 if(childpid != 0) {
 printf("Sono il processo padre con ID: %d\n",getpid());
 printf("il PID di mio figlio e': %d\n",childpid); 
 } else printf("Sono il processo figlio con ID: %d e FATHER ID: %d\n",getpid(),getppid());

 printf("sono il processo: %d e sto terminando.\n",getpid());

}
 
