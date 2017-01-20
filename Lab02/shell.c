#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAXLINE 80
#define MAXARG 20

char cmd[MAXLINE];

// Time in microseconds of all processes when the last process was run
long prevTotalTime = 0;
// Time in microseconds of all process after the current process was run
long currentTotalTime;
// Time for current process
long processTime;

// total amount of involuntary context switches when the last process was run
long prevContextSwitches = 0;

// total amount of involuntary context switches when the current process was run
long currentContextSwitches;

// involuntary context switches for the current process
long processContextSwitches;


// run given command
void background(char * cmd) {
  int i = 0;
  char * argv[MAXARG];
  char * token;
  // seperate command and arguments
  token = strtok(cmd, "\t \n");
  while(token != NULL){
    argv[i] = token;
    i++;
    token = strtok(NULL, "\t \n");
  }
  argv[i] = NULL;
  int pid = fork();
  int status;
  // run the command with arguments
  if(pid == 0) {
    execvp(argv[0], argv);
  }
  // wait for child to finish
  else {
    wait(&status);
  }

}

// get resource statistics for the previous child process
void getResourceStatistics(){
  struct rusage * r_usage = (struct rusage *) malloc(sizeof(struct rusage));
  getrusage(RUSAGE_CHILDREN, r_usage);
  // struct containing time information from getrusage
  struct timeval time_val = r_usage->ru_utime;
  // get time for current process
  currentTotalTime = time_val.tv_usec;
  processTime = currentTotalTime - prevTotalTime;
  printf("User time (µs): %ld\n", processTime);
  prevTotalTime = currentTotalTime;
  // get context switches for the current child process
  currentContextSwitches = r_usage->ru_nivcsw;
  processContextSwitches = currentContextSwitches - prevContextSwitches;
  printf("Involuntary context switches: %ld\n", processContextSwitches);
  prevContextSwitches = currentContextSwitches;
  free(r_usage);
}

int main() {
  while(1) {
    printf("mysh$ ");

    fgets(cmd, MAXLINE, stdin);

    if(!strcmp(cmd, "exit\n")) {
      exit(0);
    }
    background(cmd);
    getResourceStatistics();
  }
}

