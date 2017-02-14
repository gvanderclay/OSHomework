#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAXLINE 80
#define MAXARG 20

char cmd[MAXLINE];

void background(char * cmd) {
  int i = 0;
  char * argv[MAXARG];
  char * token;
  token = strtok(cmd, "\t \n");
  while(token != NULL){
    argv[i] = token;
    i++;
    token = strtok(NULL, "\t \n");
  }
  argv[i] = '\0';
  int pid = fork();
  int status;
  if(pid == 0) {
    execvp(argv[0], argv);
  }
  else {
    wait(&status);
  }

}

// commenting to test colors

int main() {
  struct rusage * r_usage = (struct rusage *) malloc(sizeof(struct rusage));
  long microseconds;
  while(1) {
    printf("mysh$ ");

    fgets(cmd, MAXLINE, stdin);

    if(!strcmp(cmd, "exit\n")) {
      exit(0);
    }
    getrusage(RUSAGE_CHILDREN, r_usage);
    struct timeval time = r_usage->ru_utime;
    microseconds = time.tv_usec - microseconds;
    printf("User time used: %ld\n", microseconds);
    background(cmd);
  }
  free(r_usage);
}

