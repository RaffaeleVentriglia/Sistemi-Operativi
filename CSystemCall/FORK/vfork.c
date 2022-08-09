#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int glob = 5;

int main() {
 
pid_t childpid = 0;

printf("Sono il padre %d e la mia glob ha valore: %d\n",getpid(),glob);
printf("Genero mio figlio con vfork..\n\n");

childpid = vfork();

if(childpid == 0) {
 printf("Sono il processo figlio %d generato con VFORK\n",getpid());
 printf("Ed eseguirò tutte le operazioni prima di mio padre.\n");
 for(int i = 0; i<5; i++) {
  printf("Sto per dormire per la %d volta 1 secondo.\n",i+1);
  sleep(1); 
  printf("Mi sono svegliato e aumento la variabile glob di 1\n");
  glob++;
  }
   printf("Ho finito il mio ciclo e passo il controllo a mio padre.\n\n");
   //system call exit
   _exit(EXIT_SUCCESS);
 }

 printf("Sono il padre con ID %d e ho ripreso il controllo.\n",getpid());
 printf("Valore di glob modificato da mio figlio: %d",glob);

}
