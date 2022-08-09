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

 getcwd(buffer, sizeof(buffer));

 printf("Actual directory: %s\n",buffer);
 puts("");

 if(strcmp(argv[1], buffer)) {
 safe_func("CHDIR", chdir(argv[1]));
 }

 //flowing trough the directory
 while((dir = readdir(directory)) != NULL) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
		//do nothing
	} else {
		safe_func("STAT", stat(dir->d_name, &buf));
		printf("%s execute group/user privilege: ", dir->d_name);
		if(buf.st_mode & S_IXGRP && buf.st_mode & S_IXOTH) {
			printf("YES\n");
		} else printf("NO\n");
	}
 } 

}

void safe_func(char* s, int n) {

 if(n == -1) {
	perror(s);
	exit(EXIT_FAILURE);
 }

}
