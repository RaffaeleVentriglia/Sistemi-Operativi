#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#define READ 0
#define WRITE 1

int main() {

 pid_t childpid = 0;
 int pfd, rpfd;
 char buf[32];

 //defining first pipe
 if(access("tpipe", F_OK) == 0) { //if the acess function can access the file
 	printf("TPIPE CHECK: OK\n"); //then print that the check gone well
 } else { //otherwise the pipe doesnt exists
	if(mkfifo("tpipe",0777) == 0) { //create a new pipe
		printf("TPIPE CREATION: OK\n"); //if ok, then print a message
	} else { //otherwise
		perror("PIPE"); //error 
		exit(EXIT_FAILURE); //and exit
	  }
  }

 //defining second pipe
 if(access("reversetpipe", F_OK) == 0) { //if the acess function can access the file
 	printf("REVERSETPIPE CHECK: OK\n"); //then print that the check gone well
 } else { //otherwise the pipe doesnt exists
	if(mkfifo("reversetpipe",0777) == 0) { //create a new pipe
		printf("REVERSETPIPE CREATION: OK\n"); //if ok, then print a message
	} else { //otherwise
		perror("PIPE"); //error 
		exit(EXIT_FAILURE); //and exit
	  }
  }


 //fork phase
 if((childpid = fork()) == 0) { //if the son 
	if((pfd = open("tpipe", O_RDONLY, 0777)) < 0) { //OPENING the first pipe in read
		perror("OPEN");
		exit(EXIT_FAILURE);	
	}			

	printf("SON OPENED TPIPE: OK\n");
	
 	if((rpfd = open("reversetpipe", O_WRONLY, 0777)) < 0) { //opening the reverse pipe in write
		perror("OPEN");
		exit(EXIT_FAILURE);	
	}

 	printf("SON OPENED REVERSETPIPE: OK\n");
 } else if (childpid > 0) { //if the father
	if((pfd = open("tpipe", O_WRONLY, 0777)) < 0) { //OPENING the first pipe in write
		perror("OPEN");
		exit(EXIT_FAILURE);	
	}			

	printf("FATHER OPENED TPIPE: OK\n");
	
 	if((rpfd = open("reversetpipe", O_RDONLY, 0777)) < 0) {//opening the reverse pipe in read
		perror("OPEN");
		exit(EXIT_FAILURE);	
	}

 	printf("FATHER OPENED REVERSETPIPE: OK\n");

   } 

  puts("");
 //communication phase
 if(childpid > 0) {
	if(write(pfd, "MESSAGE: WRITTEN BY FATHER", 27) < 0) {
		perror("WRITE");
		exit(EXIT_FAILURE);
	}
	
	if(read(rpfd, &buf, sizeof(buf)) < 0) {
		perror("READ");
		exit(EXIT_FAILURE);
	}
	
	printf("FATHER READING: %s\n",buf);	

	close(pfd);
	close(rpfd);

 } else {
	if(read(pfd, &buf, sizeof(buf)) < 0) {
		perror("READ");
		exit(EXIT_FAILURE);
	}
 	
 	printf("SON READING: %s\n",buf);
	
	if(write(rpfd, "MESSAGE: WRITTEN BY SON", 23) < 0) {
		perror("WRITE");
		exit(EXIT_FAILURE);
	}
	
	close(pfd);
	close(rpfd);
	
  }


}
