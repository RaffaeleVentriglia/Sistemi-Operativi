#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
 
 struct stat buf, reg_buf;
 struct dirent* dir1, *dir2;
 DIR* dirp1, *dirp2;
 DIR* currentdirp;
 char cwd[256];
 char hardlink[256];
 char to_read;
 char to_write;
 int found = 0;
 int hardlinkfd;
 int newfilefd;
 int len;
 int filesize;
 
 if(argc!=3) {
	printf("Usage: ./filename <dir1> <dir2>\n");
	exit(EXIT_FAILURE);
 }

 if(getcwd(cwd, sizeof(cwd)) == NULL) {
	perror("getcwd");
	exit(EXIT_FAILURE);
 }

 if((currentdirp = opendir(cwd)) == NULL) {
	perror("opendir current cwd");
	exit(EXIT_FAILURE);	
 }


 if((dirp1 = opendir(argv[1])) == NULL) {
	perror("opendir dirp1");
	exit(EXIT_FAILURE);	
 }

 if((dirp2 = opendir(argv[2])) == NULL) {
	perror("opendir dirp2");
	exit(EXIT_FAILURE);	
 }

 //first directory
 if(chdir(argv[1]) == -1) {
	perror("chdir to dir1");	
	exit(EXIT_FAILURE);	
 }
 
 while((dir1 = readdir(dirp1)) != NULL && found == 0) {
	if(!strcmp(dir1->d_name, ".") || !strcmp(dir1->d_name, "..")) {
		//fai niente	
	} else {
		if(lstat(dir1->d_name, &buf) == -1) {
			perror("lstat");
			exit(EXIT_FAILURE);	
		}

 		if(S_ISLNK(buf.st_mode)) {
			printf("First symlink found: %s\n", dir1->d_name);
			if((len = readlink(dir1->d_name, hardlink, strlen(dir1->d_name))) == -1) {
				perror("readlink");
				exit(EXIT_FAILURE);
			}
			printf("file pointing to: %s\n",hardlink);
			found = 1;
			hardlink[len] = '\0';
		
			if((hardlinkfd = open(hardlink, O_RDONLY, 0666)) == -1) {
				perror("open hardlink");
				exit(EXIT_FAILURE);	
			}
		
			if(stat(hardlink, &reg_buf) == -1) {
				perror("stat hardlink");
				exit(EXIT_FAILURE);	
			}
			filesize = buf.st_size;			
			
		}

	}
 }

  printf("Now switching back to previous folder..\n");
  if(chdir(cwd) == -1) {
	perror("chdir to dir2");
	exit(EXIT_FAILURE);
 }	
 
 
 printf("Now entering int the %s.\n", argv[2]);
 if(chdir(argv[2]) == -1) {
	perror("chdir to dir2");
	exit(EXIT_FAILURE);
 }	

 printf("Creating the file into %s\n",argv[2]);
 if((newfilefd = open(hardlink, O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1) {
	perror("open");
	exit(EXIT_FAILURE);
 } 

 printf("Copying the reverse of the file into the new one..\n");
 for(int i=filesize; i>=0; i--) {
	if(lseek(hardlinkfd, i, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);	
	}

	if(read(hardlinkfd, &to_read, sizeof(char)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	
	if(lseek(hardlinkfd, i, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);	
	}

	to_write = to_read;

	if(write(newfilefd, &to_write, sizeof(char)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
 }

 printf("Done\n");
 return(0);


}
