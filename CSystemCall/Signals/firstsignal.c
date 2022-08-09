#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

//Program to install new signal_handlers and try their use.
//we also define a function pointer that receive an int and return nothing
//to store previous signal handler.

typedef void (*signal_h)(int); //defining a pointer to a function that take an int e returns void
void signal_handler1(int); //our new signal handler
void signal_handler2(int); //our second new signal handler

int main() {

 signal_h prevhandler;

 //installing signal hanlder 1 to manage SIGINT, and returning the old sighand into prevhanlder.
 if(signal(SIGINT, signal_handler1) == SIG_ERR) {
 	perror("SIGNAL ERROR");
	exit(EXIT_FAILURE);
 }

 //give the time to the user to try the feature
 

 //same as above but with signale_handler 2
 if((prevhandler = signal(SIGINT, signal_handler2)) == SIG_ERR) {
	perror("SIGNAL ERROR");
	exit(EXIT_FAILURE);
 }

  //give the time to the user to try the feature
 
 //now restoring the the first handler with prevhandler
 if(signal(SIGINT, prevhandler) == SIG_ERR) {
	perror("SIGNAL ERROR");
	exit(EXIT_FAILURE);
 }

  //give the time to the user to try the feature
  while(1); 


}

void signal_handler1(int signo) {

if(signo == SIGINT) printf("RECEIVED SIGNAL: %d - 1ST HANDLER\n",signo);

}

void signal_handler2(int signo) {

if(signo == SIGINT) printf("RECEIVED SIGNAL: %d - 2ND HANDLER\n",signo);	

}
