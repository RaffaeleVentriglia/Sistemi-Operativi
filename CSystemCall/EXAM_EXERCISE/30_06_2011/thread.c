#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void* thread_directory(void*);
void thread_safe(char*, int);

char directory_name[64];
char max_filename[64];
size_t file_size = 0;
int N;

sem_t* sem_access;

int main(int argc, char* argv[]) {

 N = argc-1;
 pthread_t tids[N];
 
 if((sem_access = sem_open("sem_access", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "SEM OPEN\n");
	exit(EXIT_FAILURE);
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread_create", pthread_create(&tids[i], NULL, thread_directory, argv[i+1]));
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread join", pthread_join(tids[i], NULL));
 }
 
 printf("Max file size: %ld byte\n", file_size);
 printf("Filename: %s\n", max_filename);
 printf("Directory: %s\n", directory_name);

 sem_close(sem_access);
 sem_unlink("sem_access");
 return(0);

}

void* thread_directory(void* arg) {

 char* dir_name = (char*)arg;
 struct stat buf;
 struct dirent* dir;
 DIR* dirp;

 if((dirp = opendir(dir_name)) == NULL) {
	perror("OPENDIR");
	exit(EXIT_FAILURE);	
 }

 while((dir = readdir(dirp)) != NULL) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
		//fai niente
	} else {
		char path[128] = "./";
		strcat(path, dir_name);
		strcat(path, "/");
		strcat(path, dir->d_name);
		if(stat(path, &buf) == -1) {
			perror("stat");
			exit(EXIT_FAILURE);	
		}
		
		sem_wait(sem_access);
		if(buf.st_size > file_size) {
			file_size = buf.st_size;
			strcpy(max_filename, dir->d_name);
			strcpy(directory_name, dir_name);		
			sem_post(sem_access);		
		} else sem_post(sem_access);


	}
 }

}

void thread_safe(char* serr, int nerr) {
 
 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n",serr,nerr);
	exit(EXIT_FAILURE);
 }

}
