#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define true 1
#define false 0
void *accessFile(void*);

void handleSignalInterrupt(int);

int threadsMade = 0;

int threadsCompleted = 0;

int main() {
  signal(SIGINT, handleSignalInterrupt);
  char buffer[256];
  srand(time(NULL));
  while (true) {
    pthread_t  thread;
    int status;
    printf("Enter a filename: ");
    if (fgets(buffer, sizeof(buffer), stdin)) {
      // remove newline from end of string
      size_t ln = strlen(buffer) - 1;
      if (buffer[ln] == '\n') {
        buffer[ln] = '\0';
      }
      if((status = pthread_create(&thread, NULL, accessFile, buffer)) != 0) {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(1);
      }
      printf("Thread created for file %s\n", buffer);
      threadsMade++;
    } else {
      printf("fgets error\n");
      exit(1);
    }
  }
}

void *accessFile(void *arg) {
  char * fileName = (char *)malloc(sizeof(fileName));
  strcpy(fileName, (char *)arg);
	int delay = rand() % 5;
	if (delay < 4) {
    sleep(delay = 1);
  }
  else{
  	delay = (rand() % 4) + 7;
    sleep(delay);
  }
  char * device = delay == 1 ? "cache" : "drive";
  printf("Retrieved file: %s from %s in %d second(s)\n", fileName, device, delay);

  threadsCompleted++;
  free(fileName);
  return NULL;
}

void handleSignalInterrupt(int sigNum) {
  printf("Threads Made: %d: ", threadsMade);
  exit(1);
}

