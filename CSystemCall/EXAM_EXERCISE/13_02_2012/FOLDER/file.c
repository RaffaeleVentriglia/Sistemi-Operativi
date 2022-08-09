#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void safe_func(char*, int);

int main(int argc, char* argv[]) {

 int max_dim = 0;
 struct stat buf;
 struct dirent* dir;
 DIR* dirp;
 
 if((dirp = opendir(argv[2])) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);
 }

 if(chdir(argv[2]) == -1) {
	perror("chdir");
	exit(EXIT_FAILURE);
 }
  
 while((dir = readdir(dirp)) != NULL) {
	if(!strcmp(dir->d_name, ".") | !strcmp(dir->d_name, "..")) {
		//fai niente
	} else {
		char* name = strrchr(dir->d_name, '.');
		if(name != NULL) {
			if(!strcmp(name, argv[1])) {
				if(stat(dir->d_name, &buf) != -1) {
					printf("%s\n",dir->d_name);
					printf("Diritti di lettura per altri: ");
					if(buf.st_mode & S_IRGRP) {
						printf("SI\n");
					} else printf("NO\n");
					printf("Dimensione: %ld\n", buf.st_size);
					if(buf.st_size > max_dim) {
						max_dim = buf.st_size;
					}
				}
				puts("");
			}
		}



	}
 }


}



