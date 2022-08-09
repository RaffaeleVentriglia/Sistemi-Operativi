#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

 struct stat buf;
 struct dirent* dir;
 DIR* dirp;
 char dir_buf[64];
 char buff[256];


 if(argc!=2) {
	printf("Usage ./filename <directory>\n");
	exit(EXIT_FAILURE);	
 } else strcpy(dir_buf, argv[1]);

 if((dirp = opendir(dir_buf)) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);	
 }

 if(chdir(dir_buf) == -1) {
	perror("chdir");
	exit(EXIT_FAILURE);	
 }

 getcwd(buff, sizeof(buf));
 printf("%s\n",buff);
 
 while((dir = readdir(dirp)) != NULL) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
		//fai niente
	} else {
		stat(dir->d_name, &buf);
		if(buf.st_mode & S_IXOTH && buf.st_mode & S_IXGRP) {
			printf("file %s eseguibile da group e others.\n",dir->d_name);
			printf("eseguo il file..\n");
			if(fork()==0) {
				printf("sto per eseguire %s\n",dir->d_name);
				char buffer[64] = "./";
				strcat(buffer, dir->d_name);
				execlp(buffer, dir->d_name, NULL);
				perror("execl");
				_exit(EXIT_FAILURE);
			}			
		}
	}


 }

}
