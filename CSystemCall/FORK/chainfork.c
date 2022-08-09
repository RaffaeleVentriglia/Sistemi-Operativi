#include <unistd.h>
#include <stdio.h>

//program to create a CHAIN OF PROCESS. 

int main() {

int childpid = 0;

printf("Sono il processo padre %d e sto per creare una lista di processi (catena).\n",getpid());

for(int i=0; i<4; i++) {
 //this if will be true everytime: 
 //so we will stop a father process 
 //everytime we fork, to avoid tree process.
 if(childpid = fork()) 
  break;

 printf("Sono il processo %d, con padre %d\ncreato alla %d iterazione\n", getpid(), getppid(),i);

 }


}
