#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

int main() {

 sem_unlink("increase_written_count");
 sem_unlink("acquire_write_right");
 if(fork()==0) {
	execlp("sh", "sh" "rm TEMP", NULL);
 }


}
