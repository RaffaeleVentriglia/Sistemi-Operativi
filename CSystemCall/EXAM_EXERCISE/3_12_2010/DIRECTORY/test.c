#include <stdio.h>

int main() {
int x = 3106;
for(int i=0; i<1024; i++) {
 x+=3;
 printf("i = %d - valore %d\n",i+1039, x);
}
}
