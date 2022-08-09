#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int main() {
 
 //DUP: function that duplicate a current file descriptor into a new one;
 //this new file descriptor will point to the same output file from the duplicated one.
 
 //here we duplicate the STANDARD INPUT file descriptor, into newfd.
 //newfd can be now utilized as the STDIN (both newfd and STDIN_FILENO point to the same thing).
 int newfd = dup(STDIN_FILENO);
 char* str = "NEWFD IS THE DUP";
 
 //check if newfd is valid
 if(newfd < 0) 
 perror("DUP ERROR");

 //write to the STDIN with newfd and check if the write gone correct
 if (write(newfd, str, 18) < 0) perror("WRITE ERROR");

 //close file
 close(newfd);

//---------

//Now we open a file and with the duplicated filedes we'll write in
 int existing_fd = open("filetodup", O_WRONLY | O_APPEND);
 
 //check if this file exists
 if(existing_fd < 0) 
 perror("FILEDES ERROR");
 
 int newexisting_fd = dup(existing_fd);
  
 //check if DUP worked correctly
 if(newexisting_fd < 0) 
 perror("DUP ERROR");

 //add the line to the file, if not possible, print the error
 if(write(newexisting_fd, "LINE ADDED WITH DUP FD", 22) < 0) 
 perror("WRITE ERROR");

//close file 
 close(existing_fd);
 close(newexisting_fd);

//---------

//now we will redirect the STANDARD OUTPUT into our file with DUP2
 int file_fd = open("stdout_file", O_WRONLY);
 
 if(file_fd < 0) 
 perror("FILEDES ERROR");

 //redirect the STDOUT into our file (dup2(destination,source))
 if(dup2(file_fd, STDOUT_FILENO) < 0) 
 perror("DUP ERROR");

 if(write(STDOUT_FILENO, "TEST", 4) <0) 
 perror("WRITE ERROR");

//close file
 close(file_fd);
//-------

//Now we will redirect the keyboard input from STDIN to a new created file

//creating a new file
 int stdin_fd = open("stdin_file", O_RDWR | O_APPEND);
 char buf[5];

 if (stdin_fd < 0)  
 perror("FILEDES ERROR");

 //redirecting the stdin to file des
 if (dup2(stdin_fd, STDIN_FILENO) < 0)
 perror("DUP ERROR");

 //reading something from the STDIN_FILENO, now our standard input is taken from the file stdin_file.
 if(read(STDIN_FILENO, buf, 5) < 0)
 perror("READ ERROR");

 //WRITING SOMETHING on the standard input, the file stdin_file.
 if(write(STDIN_FILENO, buf, 5) <0)
 perror("WRITE_ERROR");
 
 //close file 
 close(stdin_fd);

 
}
