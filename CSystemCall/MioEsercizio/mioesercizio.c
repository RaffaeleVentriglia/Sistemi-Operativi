#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//Objective:
//1) create a new input file, output file and redirected output file
//2) write a string on it (at least 16char)
//3) duplicate the filedes of the input file and use that to operate 
//4) use lseek to move to the 5th character of the string
//5) write from the 5th character till the end of the file output


int main () {

//declaring filedescriptor and useful variables
 int fdin, fdout,redfdin, redfdout;
 int nbyteread;
 char buffer[32];
 char newbuffer[32];

//opening phase 
 if((fdin = open("file_in", O_CREAT | O_RDWR | O_TRUNC | O_EXCL, 0600)) < 0) {
 perror("FILEDES ERROR");
 exit(EXIT_FAILURE); 
 }

 if((fdout = open("file_out", O_CREAT | O_RDWR | O_TRUNC | O_EXCL, 0600)) < 0) {
 perror("FILEDES ERROR:");
 exit(EXIT_FAILURE);
 }

//write a string from standard input to the file
 if((nbyteread = read(STDIN_FILENO, buffer, 32)) > 0) {
  if(write(fdin, buffer, nbyteread) < 0) {
   perror("WRITE ERROR");
   exit(EXIT_FAILURE);
  }
 } else {
   perror("READ ERROR");
   exit(EXIT_FAILURE);
}


//NB: the buffer will be automatically cleaned each time we'll call a read 

//now duplicating the filedes of fdin:
//and checking the integrity
 if((redfdin = dup(fdin)) < 0) {
 perror("FILEDES ERROR");
 exit(EXIT_FAILURE);
 }


//using lseek to move to the 5th character from the beginning,
//the using the read to read from the duplicated filedes the text into fdin, 
//that is now setted at the 5th character (OFFSET moved by 5 from lseek) 
//and then write into fdout the rest of the string.
 if((lseek(redfdin, 5, SEEK_SET)) > 0) {
  if((nbyteread = read(redfdin, buffer, 32)) > 0) {
   if(write(fdout, buffer, nbyteread) < 0) {
    perror("WRITE ERROR");
    exit(EXIT_FAILURE);
    }
   } else {
       perror("READ ERROR");
       exit(EXIT_FAILURE);
    }
  }

//we need no more fdin, close it
close(fdin);
close(redfdin);

//After using WRITE to write into the output file, the offset of FDOUT
//is set at the end (0). We need, before to read what's in file_out, 
//reset the offset to 0, giving the possibility to read correctly.
 if(lseek(fdout, 0, SEEK_SET) < 0) {
 perror("LSEEK ERROR:");
 exit(EXIT_FAILURE);
 } 

//printing the file fdout
 if((nbyteread = read(fdout, newbuffer, 1)) > 0) {
  if(write(STDIN_FILENO, newbuffer, nbyteread) < 0) {
   perror("WRITE ERROR");
   exit(EXIT_FAILURE);
   } 
  } else {
    perror("READ ERROR:");
    exit(EXIT_FAILURE);
    }


//now we'll redirect from the output file to the STDOUT.
 if((redfdout = dup(fdout)) < 0) {
 perror("FILEDES ERROR");
 exit(EXIT_FAILURE);
 }

if(dup2(redfdout, STDOUT_FILENO) < 0) perror("DUP ERROR:");
write(redfdout, "test", 4);


}

