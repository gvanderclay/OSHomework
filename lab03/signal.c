#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
void sigHandler(int);
void sigHandler2(int);
int main () {
    signal (SIGINT, sigHandler);
    signal (SIGTSTP, sigHandler2);
    pid_t ppid = getpid();
    pid_t pid;
    srand(time(NULL));
    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    }
    
    if(pid != 0){
    	while(1){
    		printf ("waiting...  ");
    		fflush(stdout);
    		pause ();
    	}
    	
    }
    else{ 
    //send random signal from child
    	while(1){
    		int r1 = rand()%2;
    		int r2 = rand()%5;
    		sleep(r2);
    		if(r2 == 0){
    			kill(ppid, SIGINT);
    			exit(0);
    		}
    		else{
    			kill(ppid, SIGTSTP);
    		}
    	}
    	
    }
   
    return 0;
}

void sigHandler (int sigNum) {
    printf (" received SIGINT (CTRL-C)\n");
    printf ("Shutting down parent gracefully\n");
    // this is where shutdown code would be inserted 
    exit(0);
    
}
void sigHandler2(int sigNum) {
		printf (" received SIGSTP (CTRL-Z)\n");
    // this is where shutdown code would be inserted 
}
