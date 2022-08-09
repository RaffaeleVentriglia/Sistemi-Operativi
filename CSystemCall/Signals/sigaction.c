#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

typedef void (*sig_han)(int);
static void signal_handler(int);

int main() {

 sig_han newhandler = NULL;
 
 //creating two sigaction struct
 struct sigaction act,oact;

 //creating the sigset_t type for the mask
 sigset_t set;

 //define the signal set: this is useful to what you could ask?
 //is useful to prevent the signal overlapping from the same signal type
 //we're defining action for, or for different types.  
 //basically with a defined set of signal with sigset_t and sigaction, we're avoiding race.

 //so here we're defining all the signals that we dont want to mess with our handler when 
 //he's in esecution.
 sigfillset(&set);

 //assigning the address of the signal_handler function to sig_han
 newhandler = &signal_handler;

 //define sigaction structure
 act.sa_handler = newhandler;
 act.sa_mask = set;
 act.sa_flags = SA_RESTART;

 //call sigaction 
 sigaction(SIGINT, &act, &oact);

 //test if works
 while(1);
 
}
static void signal_handler(int signo) {
 
 //CAREFUL: WE NEED TO PUT THE NEWLINE TO FLUSH THE BUFFER, OTHERWISE IT WILL NOT PRINT ANYTHING.
 printf("CTRL+C DISABLED\n");

}

