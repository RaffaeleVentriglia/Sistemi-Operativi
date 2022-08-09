#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {

int fdin, fdout;
char buffer[4],outbuffer[1],sum;
int n,nbyte;

//if we passed an argument through terminal, 
//the program will open our specified file.
if(argc >= 2)
fdin = open(argv[1],O_RDONLY);
//otherwise standard file
else fdin = open("filetest",O_RDONLY);

//create an output file 
fdout = open("outputfile", O_WRONLY | O_CREAT | O_TRUNC, 0600);

//error if one of those two file couldnt be opened / found
if(fdin < 0 || fdout < 0) 
perror("write error");

 //while the file is being read, and the end of the file
 //isn't reached
 while((nbyte = read(fdin,buffer,4)) > 0) {
 //if we encounter -1 at some point
  if(buffer[0] == '-' && buffer[1] == '1') {
   //stop
   write(fdout, "-1 STOP", 7);
   exit(0);
  }
  
  //actions to translate char to int and sum int to char
  n = (buffer[0] - '0') + (buffer[2] - '0');
  sum = n + (int)'0';
  outbuffer[0] = sum;
  outbuffer[1] = '\n';
  
  //check that the byte written on the read are correct: 
  //we need to write nbyte/2 (number+ \n)
  if(write(fdout, outbuffer,2) != nbyte/2) {
   //error otherwise
    printf("write error");
    exit(0);
    }
  }

//closing file 
close(fdin);
close(fdout);

}
