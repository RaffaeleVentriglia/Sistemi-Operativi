#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char* argv[]) {

 //check if the argc is > 2, otherwise exit
 if(argc!=3) {
 printf("ONLY THREE ARGUMENT ARE REQUIRED");
 exit(-1);  
 }

 char buffer[128];
 DIR* directory; 
 DIR* basedir;
 struct dirent* dirstruct;
 
 //printing the working directory
 if(getcwd(buffer,sizeof(buffer)) < 0) {
 perror("GETCWD ERROR");
 exit(EXIT_FAILURE);
 } else printf("CWD IS: %s\n",buffer);


 //open the root directory for base use
 if((basedir = opendir("/")) != NULL) {
 printf("BASE DIRECTORY ASSIGNED.\n");
 } else {
   perror("OPENDIR");
   exit(EXIT_FAILURE);
  }

 //open a new directory 
 if((directory = opendir(argv[1])) != NULL) {
 printf("DIRECTORY CORRECTLY OPENED: RETRIEVING INFO..\n");
 } else { 
   perror("OPENDIR:");
   exit(EXIT_FAILURE);
   }

 printf("FILES INTO THE DIRECTORY:\n");
 //reading the new directory information then print the info 
  while((dirstruct = readdir(directory)) != NULL) {
   if(!strcmp(dirstruct->d_name, ".") || !strcmp(dirstruct->d_name, "..")) {
     //lmao do nothing cause it's a fkn .
     } else {
	printf("NAME: %s\n", dirstruct->d_name);
       } //end else
    } //end while

 
/// here crash the program: maybe because it reads a null value into the struct
///giving a segfault error. correct that. 
///i dont know if the code below this comment works too, need to correct first this
/// error.


 //we dont need anymore this directory
 closedir(directory);

 //now rewind to the base directory 
 rewinddir(basedir);  
 
 //now changing directory and going into something different with 
 //chdir:
 if(chdir(argv[2]) <0) {
  perror("CHDIR");
  exit(EXIT_FAILURE);
 } else printf("CHDIR TO %s DONE... RETRIEVING INFO..\n",argv[2]);
 

} //end main
