#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//first process using the file "Myfifo". 
//This will be the reader process.

int main() {

 //Initialization
 char buffer[32];
 int pipefd;
 int nbyte;
 
 //let's open our fifo
 if((pipefd = open("myfifo", O_RDONLY | O_NONBLOCK , 0777)) < 0) {
	perror("OPEN");
	exit(EXIT_FAILURE);
 }

 printf("aperta\n");
 sleep(5);
 //let's read from our fifo
 if((nbyte = read(pipefd, &buffer, sizeof(buffer))) < 0) {
	perror("READ");	
	exit(EXIT_FAILURE);
 }

 //let's print the information 
 printf("%s\nByte read: %d\n",buffer,nbyte);


}
