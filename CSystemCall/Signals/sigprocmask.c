#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int critical_counter = 0;

static void signal_handler(int);

int main() {

//setting the handler
  signal(SIGINT, signal_handler);
  while(1);

}

static void signal_handler(int signo) {

 sigset_t set, oset;

 //insert all signals into set
 sigfillset(&set);
 //call sigprocmask to set the new blocking mask for all the signals
 sigprocmask(SIG_SETMASK, &set, &oset);

 //now we can enter into the critical section and modify values.
 //note: we're not using SIGACTION(), so the race condition is not fully avoided.
 //CS:
   printf("INTO CRITICAL SECTION: COUNTER VALUE: %d - INCREMENTING..\n",critical_counter);
   critical_counter++;
   printf("INTO CRITICAL SECTION: COUNTER VALUE AFTER INCREMENTING: %d\n",critical_counter);

 //now restoring the old mask, because we're out of the CS.
 sigprocmask(SIG_SETMASK, &oset, NULL);

 //returning to main...


}
