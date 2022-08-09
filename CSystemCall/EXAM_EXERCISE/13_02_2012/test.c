#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

 struct stat buf;
 struct dirent* directory;
 DIR* dir;

 dir = opendir(argv[1]);
 chdir(argv[1]);
 while((directory = readdir(dir)) != NULL) {
	if(!strcmp(directory->d_name, ".") | !strcmp(directory->d_name, "..")) {
		//do nothing
	} else 	{	
		char* name = strrchr(directory->d_name, '.');		
		if(name != NULL) {
			if(!strcmp(name, ".c")) {
				printf("%s is a c file.\n", directory->d_name);
			}
		}
	  } 
 }

}
