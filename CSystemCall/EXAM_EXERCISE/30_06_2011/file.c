#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#define WRITE 1
#define READ  0
#define MAX_SIZE 100

struct message {
 size_t bytes;
 char fname[MAX_SIZE];
};

typedef struct message s_mes;

int get_biggest_file(int, int, char*);

int main(int argc, char* argv[]) {

 pid_t pid;
 size_t biggest_bytes = 0;
 int N = argc-1; //number of the childs to create
 int ROW = 0;
 int biggest;
 int pipe_array[N][2]; //array of pipes
 s_mes to_read[N];
 //creating N pipes for the process
 for(int i=0; i<N; i++) {
	if(pipe(pipe_array[i]) != 0) {
		perror("PIPE");
		exit(EXIT_FAILURE);
	}
 }

 //creating N process and giving them the pipes
 for(int i=0; i<N; i++) {
	if((pid = fork()) == 0) {
		ROW = i;
		close(pipe_array[i][READ]);
		break;	
	} else if (pid > 0) {
		close(pipe_array[i][WRITE]);
	  }
 }

 if(pid==0) get_biggest_file(ROW, pipe_array[ROW][WRITE], argv[ROW+1]);
 else {
	while(wait(NULL) != -1);
 }

 //deciding who's the bigger file:
 for(int i=0; i<N; i++) {
	read(pipe_array[i][READ], &to_read[i], sizeof(s_mes));
	if(to_read[i].bytes > biggest_bytes) {
		biggest_bytes = to_read[i].bytes;
		biggest = i;
	}
	close(pipe_array[i][READ]);
 }
 
 puts("");
 printf("Father PID: %d\nFolder checked: %d\nBiggest element name: %s\nBiggest element size: %lu\n",getpid(), N, to_read[biggest].fname, to_read[biggest].bytes);

}

int get_biggest_file(int ROW, int PIPEFD, char* FOLDER) {

 DIR* dirp;
 s_mes to_send;
 struct dirent* directory;
 struct stat buf;
 size_t biggest_file = 0;
 char name[MAX_SIZE];
 
 //opening dir
 if((dirp = opendir(FOLDER)) == NULL) {
	perror("OPENDIR");
	exit(EXIT_FAILURE);
 }

 //changing dir
 if(chdir(FOLDER) == -1) {
	perror("CHDIR");
	exit(EXIT_FAILURE);
 }

 //now reading the dir to find out which file is the biggest, and returning the bytes to the father.
 while((directory = readdir(dirp)) != NULL) {
	if(!strcmp(directory->d_name, ".") || !strcmp(directory->d_name, "..")) {
		//do nothing if . or ..
 	} else {
		if(stat(directory->d_name, &buf) == -1) {
			perror("STAT");	
			exit(EXIT_FAILURE);
		}
		if(buf.st_size > biggest_file) {
			biggest_file = buf.st_size;
			strcpy(name, directory->d_name);
		}	 
	}
 }

 //updating the structure
 sprintf(to_send.fname, "%s", name);
 to_send.bytes = biggest_file;

 //passing the result to the father
 write(PIPEFD, &to_send, sizeof(to_send));
 close(PIPEFD);
 _exit(EXIT_SUCCESS);	







}

