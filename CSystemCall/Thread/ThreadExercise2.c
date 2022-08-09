#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>

struct thread_struct {
 int t_id;
 int beginning;
};

typedef struct thread_struct t_str;

void ERR_SYS(char*, int);
void* find_occurrence(void*);

int filepart;
int gfd;
char to_find;

int main(int argc, char* argv[]) {

 int N = atoi(argv[1]);
 int fd, filesize, err, totaloccur = 0;
 void* retval;
 struct stat buf;
 pthread_t tids[N-1];
 t_str thread_util[N-1];

 
 to_find = *(char*)argv[3];
 
 //open the file
 if((fd = open(argv[2], O_RDONLY , 0777)) == -1) {
	perror("OPEN");
	exit(EXIT_FAILURE);
 } 

 gfd = fd;

 //check it's size 
 if(fstat(fd, &buf) == -1) {
	perror("FSTAT");
	exit(EXIT_FAILURE);
 }
 
 filesize = buf.st_size; 
 filepart = filesize/N;
  

 //creating N threads
 for(int i=0; i<N; i++) {
	thread_util[i].t_id = i;
 	thread_util[i].beginning = i*filepart;
 	if((err = pthread_create(&tids[i], NULL, find_occurrence, &thread_util[i])) != 0) 
		ERR_SYS("PTHREAD_CREATE", err);

 }

 for(int i=0; i<N; i++) {
	if((err = pthread_join(tids[i], &retval)) != 0) 
		ERR_SYS("PTHREAD_JOIN", err);

	totaloccur += *(int*)retval;
 }

 printf("Total occurrence of %s: %d\n",argv[3], totaloccur);

}

void ERR_SYS(char* strerror, int errnum) {
	
 fprintf(stderr, "%s : %d\n", strerror, errnum);
 exit(EXIT_FAILURE);

}

void* find_occurrence(void* arg) {

 t_str thread_info = *(t_str*)arg;
 int beginning = thread_info.beginning;
 int limit = beginning+filepart-1;
 int occurrnum = 0, byteread = 0, r;
 char occur;

 printf("T: %d - Beginning byte: %d - limit byte: %d\n",thread_info.t_id, beginning,limit);
 static int occurrence;

 //positioning the offset 
 if(lseek(gfd, beginning, SEEK_SET) == -1) {
	perror("LSEEK");
	exit(EXIT_FAILURE);
 }
 

 //finding the occurrence
 while(byteread < limit) {

	if(lseek(gfd, beginning+byteread, SEEK_SET) == -1) {
		perror("LSEEK");
		exit(EXIT_FAILURE);
	}

	if((r = read(gfd, &occur, sizeof(char))) == 1) {
		if(occur == to_find) {
			occurrnum++;	
			printf("T: %d - %c equal to %c: occurr num %d\n",thread_info.t_id, occur, to_find,occurrnum);	
		}
		printf("Byteread: %c\n",occur);
		byteread++;
		printf("Thread %d byte read so far: %d\n",thread_info.t_id, byteread);
	} 

	
 } 

 occurrence = occurrnum;
 //returning the occurrence number found
 pthread_exit((void*)&occurrence);
 

}
