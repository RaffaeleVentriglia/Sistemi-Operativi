#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 32

int main() { 

char buffer[2];
char sum;
int n;


while(read(STDIN_FILENO, buffer, 2)>0) {

n = (buffer[0]-'0') + (buffer[1]-'0');
sum = n + (int)'0';
buffer[2] = sum;
write(STDOUT_FILENO, "

 } 
}
