#include <unistd.h>
#include <stdio.h>

//program to create a process tree

int main() {

 int childpid = 0;
  
 printf("Sono il processo padre: %d\ne sto per creare un albero dei processi.\n",getpid());

 for(int i = 0; i<4; i++) {
 //now we will create a process tree
 childpid = fork();
  
 printf("Sono il processo %d, con padre %d\ncreato alla %d iterazione.\n\n",getpid(),getppid(),i); 

 }


}
