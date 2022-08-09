#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//first process using the file "Myfifo". 
//This will be the writer process.

int main() {

 //Initialization 
 int fifofd;
 char str[32];


 //let's open the fifo
 if((fifofd = open("myfifo", O_WRONLY, 0777)) < 0) {
	perror("OPEN");
	exit(EXIT_FAILURE);
 }

 //scan a string to insert into the fifo.
 fgets(str,32,stdin);

 //now let's write into the fifo
 if(write(fifofd, &str, strlen(str)) != strlen(str)) {
	perror("WRITE");
	exit(EXIT_FAILURE);
 }
 



}


