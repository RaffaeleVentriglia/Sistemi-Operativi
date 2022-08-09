#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>



void* thread_directory(void*);
void thread_safe(char*, int);

char min_file[64];
int min_dim = 999999;
int N;
sem_t* sem_access;


int main(int argc, char* argv[]) {
 
 int N = argc-1;
 pthread_t tids[N];
 
 if((sem_access = sem_open("sem_access", O_CREAT | O_EXCL, 0666, 1)) == NULL) {
	fprintf(stderr, "SEM_ACCESS ERRROR\n");
	exit(EXIT_FAILURE);
 }

 for(int i=0; i<N; i++) {
	thread_safe("pthread_create", pthread_create(&tids[i], NULL, thread_directory, argv[i+1]));
 }
 
 for(int i=0; i<N; i++) {
	thread_safe("pthread_join", pthread_join(tids[i], NULL));
 }
 
 printf("File .txt con dimensione minore: %s di %d byte.\n", min_file, min_dim);
 sem_close(sem_access);
 sem_unlink("sem_access");
 return(0);

}

void* thread_directory(void* arg) {

 struct stat buf;
 struct dirent* dir;
 DIR* dirp;

 if((dirp = opendir((char*)arg)) == NULL) {
	perror("opendir");
	exit(EXIT_FAILURE);	
 } 

 while((dir = readdir(dirp)) != NULL) {
	if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {	
		//fai niente
	} else {
		char* end = strrchr(dir->d_name, '.');
		if(end != NULL) {
			if(!strcmp(".txt", end)) {
				char path[256] = "./";
				strcat(path, (char*)arg);
				strcat(path, "/");
				strcat(path, dir->d_name);
				if(stat(path, &buf) == -1) {
					perror("stat");
					exit(EXIT_FAILURE);
				}

				printf("%s PERMESSI LETTURA %d /SCRITTURA %d /ESECUZIONE %d UTENTE\n",dir->d_name, buf.st_mode & S_IRUSR, buf.st_mode & S_IWUSR, buf.st_mode & S_IXUSR);
			        printf("%s PERMESSI LETTURA %d /SCRITTURA %d /ESECUZIONE %d GRUPPO\n",dir->d_name, buf.st_mode & S_IRGRP, buf.st_mode & S_IWGRP, buf.st_mode & S_IXGRP);
				printf("%s PERMESSI LETTURA %d /SCRITTURA %d /ESECUZIONE %d ALTRI\n\n",dir->d_name, buf.st_mode & S_IROTH, buf.st_mode & S_IWOTH, buf.st_mode & S_IXOTH);      				
				if(buf.st_size < min_dim) {
					sem_wait(sem_access);
					sprintf(min_file, "%s", dir->d_name);
					min_dim = buf.st_size;
					sem_post(sem_access);
				}
			}
			
		}
	}	 
 }

}


void thread_safe(char* serr, int nerr) {

 if(nerr != 0) {
	fprintf(stderr, "%s: %d\n", serr, nerr);
	exit(EXIT_FAILURE);	
 } 

}
