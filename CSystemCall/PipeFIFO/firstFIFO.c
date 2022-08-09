#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


int main() {

 //Initialization
 //to check if the mkfifo returns EEXIST as error due to O_EXCL
 
 if(mkfifo("myfifo", 0777) == 0) {
	printf("FIFO CORRECTLY CREATED.");
 } else if (errno == EEXIST) {
	printf("FIFO ALREADY EXISTENT");
   } else {
	printf("%d",errno);	
	perror("FIFO");
	exit(EXIT_SUCCESS);
     }


}
