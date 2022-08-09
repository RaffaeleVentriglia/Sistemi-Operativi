#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {


struct stat buf;
//we need a struct dirent pointer cus the function readdir returns a pointer
struct dirent* stdir;
//same as above: opendir returns a pointer
DIR* directory;
//buffer for the cwd
char buffer[128];
//buffer for the file
char fbuffer[256];
//file descriptor 
char realname[256];
int fd,nbyte;

//NB: TO CHECK IF A LINK IS A SYMLINK WITH LSTAT, 
//AND THE FUNCTION S_ISLNK, WE ABSOLUTELY NEED TO 
//CHECK THAT IN THE CURRENT WORKING DIRECTORY, 
//OTHERWISE OUR PROGRAM WILL FIND NOTHING. 
//SO, WE NEED TO CHANGE THE DIRECTORY BEFORE
//EXECUTING THE ANALYSIS, OTHERWISE WE'LL GET NOTHING.
//SAME FOR THE OTHER STAT/LSTAT/FSTAT ANALYSIS,
//AND I THINK THAT CHANGING DIRECTORY TO MAKE THINGS
//WORKS IS A PREROGATIVE OF ALL THIS FUNCTIONS.

 //if there is something into the argv array
 if(argc==2) {
  //than change folder to the preferred one
  chdir(argv[1]);
  //then get the cwd into a buffer
  getcwd(buffer,sizeof(buffer));
  } else
     //else dont care, get the actual cwd 
     getcwd(buffer,sizeof(buffer));

//open the directory we want to check the presence of SYMLINK
if((directory = opendir(buffer)) != NULL) {
 printf("DIRECTORY %s CORRECTLY OPENED.\n",buffer);
 } else {
   perror("OPENDIR ERROR");
   exit(EXIT_FAILURE);
   }

printf("SYMLINK INTO THIS DIRECTORY:\n");
//print all the info about the file into the directory
 while((stdir = readdir(directory)) != NULL) {
 if(!strcmp(stdir->d_name, ".") || !strcmp(stdir->d_name, "..")) {
 //do nothing lmao
 } else {
    if(lstat(stdir->d_name, &buf) == 0) {
     if(S_ISLNK(buf.st_mode) > 0) {
      printf("SYMBOLIC FILE: %s\n",stdir->d_name);
      printf(" -CONTENT OF FILE: ");
      fflush(stdout);
      fd = open(stdir->d_name, O_RDONLY);
      nbyte = read(fd, fbuffer, sizeof(buf));
      write(STDOUT_FILENO, fbuffer, nbyte);
      close(fd);
      readlink(stdir->d_name,realname, sizeof(realname));
      printf("POINTING TO FILE: %s\n",realname);  
     } //end internal if
    } //end external if
   } //end else 
  } //end while 


closedir(directory);

}


