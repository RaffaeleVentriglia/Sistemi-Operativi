#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void analyzeSYMLINK(char* name, int size, int* bytesize, char** b_reg, char** b_sym);
void point(int* a);

int main(int argc, char* argv[]) {
//variables
 //stat struct
 struct stat statbuf;
 //dirent struct
 struct dirent* stdir;
 //DIR directory for opening a new one
 DIR* directory;
 //buffer for the cwd
 char buffer[128];
 //name of the biggest regular file
 char biggestRegoular[32];
 //name of the biggest symlink
 char biggestSymlink[32];
 //size checker 
 int bytesize = 0;

 //open the cwd
 getcwd(buffer,sizeof(buffer));


 //lets open the specified directory 
 if((directory = opendir(buffer)) < 0) {
 perror("OPENDIR ERROR:");
 exit(EXIT_FAILURE);
 } else {
    printf("DIRECTORY %s CORRECTLY OPENED\n", buffer); 
   }

 while((stdir = readdir(directory)) != NULL) {
 //lets check the stat buffer: if we analyze stat with succes
 if(lstat(stdir->d_name, &statbuf) == 0) {
  //if the actual file is a symbolic link
   if(S_ISLNK(statbuf.st_mode) > 0) {
     //pass to function the needed values
     analyzeSYMLINK(stdir->d_name, statbuf.st_size, &bytesize, biggestRegoular, biggestSymlink);
    } //end internal if
   } //end external if
  } //end while


printf("BIGGEST SYMLINK POINTING REGOULAR FILE IS: %s\n", biggestSymlink);
printf("POINTING TO: %s\n", biggestRegoular); 
printf("OF SIZE: %d\n", bytesize);


}



void analyzeSYMLINK(char* name, int size, int* bytesize, char biggest[], char bsymlink[]) {

struct stat analyzebuf;
char buffer[32];

 if(readlink(name, buffer, sizeof(buffer)) < 0) {
  perror("READLINK ERROR:");
  exit(EXIT_FAILURE);
  }
 
 //now analyzing the regoular file
 stat(buffer, &analyzebuf);
 
 //if the file size is bigger
 if(S_ISREG(analyzebuf.st_mode) && analyzebuf.st_size > size) {
 *bytesize = analyzebuf.st_size;
 strcpy(bsymlink, name);
 strcpy(biggest, buffer);
 }

}

