#include <stdio.h>
#include <unistd.h>

int main() {
 
 if(fork()==0) {
	execlp("./test", "test", NULL);
 }

 if(fork()==0) {
	execlp("./FOLDER/testfolder", "testfolder", NULL);
 }	


}
