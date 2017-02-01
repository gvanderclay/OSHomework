#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
void shutdownHandler(int);
void sigUSRHandlder(int);

pid_t pid;

int main () {
    signal(SIGUSR1, sigUSRHandlder);
    signal(SIGUSR2, sigUSRHandlder);

    // Store the pid of the parent
    pid_t ppid = getpid();
    // initial seed for random
    srand(time(NULL));
    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    }

    if(pid != 0){
      signal (SIGINT, shutdownHandler);
      printf("spawned child PID# %d\n", pid);
    	while(1){
    		printf ("waiting...  ");
    		fflush(stdout);
    		pause();
    	}

    }
    else{
    //send random signal from child
    	while(1){
        // choose between two signals
    		int signalChoice = rand() % 2;
        // random time to sleep
    		int sleepTime = rand() % 5;
        // sleep for the random time
    		sleep(sleepTime);
        // choose which SIGUSR signal to send
    		if(signalChoice == 0){
          kill(ppid, SIGUSR1);
    		}
    		else{
          kill(ppid, SIGUSR2);
    		}
    	}
    }

    return 0;
}

void shutdownHandler (int sigNum) {
  printf (" received\n");
  printf ("That's it. I'm shutting you down...\n");
  // kill the child
  kill(pid, SIGKILL);
  exit(0);
}

void sigUSRHandlder(int sigNum) {
  char * signalName;
  if(sigNum == 10) {
    signalName = "SIGUSR1";
  }
  else {
    signalName = "SIGUSR2";
  }
  printf (" received a %s signal\n", signalName);
}
