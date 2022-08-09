#include <stdio.h>
#include <fcntl.h>
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
 char max_file[64];
 char filename[64];
 int fd;
 struct stat buf;
 struct dirent* dir;
 DIR* dirp, *dirp1;
 
 if((dirp = opendir(argv[1])) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);
 }

 if((dirp1 = opendir(argv[1])) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);
 }

 if(chdir(argv[1]) == -1) {
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
					if(buf.st_mode & S_IROTH) {
						printf("SI\n");
					} else printf("NO\n");
					printf("Dimensione: %ld\n", buf.st_size);
					if(buf.st_size > max_dim) {
						strcpy(max_file, dir->d_name);
						max_dim = buf.st_size;
					}
				}
				puts("");
			}
		}

	}
 }


 for(int i=0; i<strlen(max_file); i++) {
	if(max_file[i] == '.') {
		break;
	}
	filename[i] = max_file[i];

 }

 strcat(filename, ".txt");

 if((fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1) {
	perror("open");	
	exit(EXIT_FAILURE);
 }
 
 while((dir = readdir(dirp1)) != NULL) {
	if(!strcmp(dir->d_name, ".") | !strcmp(dir->d_name, "..")) {
		//fai niente
	} else {
		char* to_write = dir->d_name;
		strcat(to_write, "\n");
		if(write(fd, to_write, strlen(dir->d_name)) < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
 }


}



