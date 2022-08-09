#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int main() {

 sem_unlink("put");
 sem_unlink("get");
 if(fork() == 0) {
	execlp("./semcreate", "./semcreate", "-e", "-n", "put", "-v" "10", NULL);	
 } else {
	if(fork() == 0) {
		execlp("./semcreate", "./semcreate", "-e", "-n", "get", "-v", "0",NULL);
	}
   }


}
