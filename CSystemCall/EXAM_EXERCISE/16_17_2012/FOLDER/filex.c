#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MAX_SIZE 150

void safe_func(char*, int);

int main(int argc, char* argv[]) {

 char buffer[MAX_SIZE];
 struct stat buf;
 struct dirent* dir;
 DIR* directory;

 if((directory = opendir(argv[1])) == NULL) {
	perror("OPENDIR");
	exit(EXIT_FAILURE);
 }

 getcwd(buffer, sizeof(buf));

 if(strcmp(argv[1], buffer)) {
 safe_func("CHDIR", chdir(argv[1]));
 }

 //assigning the directory 
 while((dir = readdir(directory)) != NULL) {
	printf("%s\n",dir->d_name);
 } 

}

void safe_func(char* s, int n) {

 if(n == -1) {
	perror(s);
	exit(EXIT_FAILURE);
 }

}
