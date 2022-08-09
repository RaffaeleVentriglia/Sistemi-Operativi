#include <stdio.h>
#include <string.h>

int main() {

char* dir = "DIR1";
char path[256] = "./";
strcat(path, dir);
strcat(path, "/");
strcat(path, "FILE");
printf("%s\n",path);

}
