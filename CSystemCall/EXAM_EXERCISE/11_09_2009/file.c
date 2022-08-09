#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {

 struct stat buf;
 struct dirent* dir;
 DIR* dirp, *dirp1;
 char buffer[256];
 char hardlink[256];
 char hlink_dir[256];
 int found = 0;
 int len;
  
 if(argc!=2) {
	printf("Usage: ./filename <filetosearch>\n");
	exit(EXIT_FAILURE);
 }

 if(getcwd(buffer, sizeof(buffer)) == NULL) {
	perror("getcwd");
	exit(EXIT_FAILURE);	
 }

 if((dirp = opendir(buffer)) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);	
 }	

 dirp1 = dirp;

 while((dir = readdir(dirp)) != NULL && found == 0) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
		//FAI NIENTE
	} else {
		if(!strcmp(dir->d_name, argv[1])) {
			found = 1;			
			if(stat(argv[1], &buf) == -1) {
				perror("stat");
				exit(EXIT_FAILURE);
			}
			printf("FILE TROVATO.\n");		
		}
	}

 }
 

 if(S_ISREG(buf.st_mode) && buf.st_size > 1) {
	printf("Filesize > 1KB, e' regolare..\n");
	found = 0;				
 } else {
	printf("FIle non idoneo.\n");		
	exit(EXIT_FAILURE);
 }

 //trovare il file simbolico
 while((dir = readdir(dirp1)) != NULL && found == 0) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
 		//fai niente
	} else {
		if(lstat(dir->d_name, &buf) == -1) {
			perror("lstat");
			exit(EXIT_FAILURE);
		}	
		if(S_ISLNK(buf.st_mode)) {
			found = 1;
			len = readlink(dir->d_name, hardlink, sizeof(hardlink));
			hardlink[len] = '\0';
			char* res = realpath(hardlink, hlink_dir);
			if(res!=NULL) {
				printf("LINK SIMBOLICO: %s\n",dir->d_name);
				printf("PUNTA AL FILE: %s\n", hardlink);
				printf("CONTENUTO IN DIRECTORY: %s\n", hlink_dir);
				found = 0;
			} else {
				printf("SYMFILE CORROTTO.\n");
				exit(EXIT_FAILURE);
			}
		
		}
	}
 }



}
