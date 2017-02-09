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

// Amount of threads that were made
int threadsMade = 0;

// Amount of threads that are currently running
int threadsRunning = 0;

// Amount of time threads spend accessing files
int fileAccessTime = 0;

// Lock used to handle multiprocessing
pthread_mutex_t lock;

int main() {

  signal(SIGINT, handleSignalInterrupt);

  char buffer[256];

  srand(time(NULL));

  if(pthread_mutex_init(&lock, NULL) != 0) {
    printf("Error making mutex\n");
    return 1;
  }

  while (true) {
    pthread_t  thread;
    int status;

    printf("Enter a filename: ");
    // Gather input
    if (!fgets(buffer, sizeof(buffer), stdin)) {
      printf("fgets error\n");
      exit(1);
    }
    else {
      // remove newline from end of string
      size_t ln = strlen(buffer) - 1;
      if (buffer[ln] == '\n') {
        buffer[ln] = '\0';
      }
      // create thread with accessfile function and buffer as the arg
      if((status = pthread_create(&thread, NULL, accessFile, buffer)) != 0) {
        fprintf(stderr, "thread create error %d: %s\n", status, strerror(status));
        exit(1);
      }
      printf("Thread created for file %s\n", buffer);
      threadsMade++;
      threadsRunning++;
    }
  }
}

void *accessFile(void *arg) {
  char * fileName = (char *)malloc(sizeof(fileName));
  strcpy(fileName, (char *)arg);

  // Handling sleeping for random amount
  // 80% chance for 1 second
  // 20% chance for 7 to 10 seconds
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

  // lock the data with a mutex
  pthread_mutex_lock(&lock);
  threadsRunning--;
  fileAccessTime += delay;
  // unlock the data
  pthread_mutex_unlock(&lock);
  free(fileName);
  return NULL;
}

void handleSignalInterrupt(int sigNum) {
  // reset the SIGINT signal
  signal(SIGINT, exit);
  // wait till all threads are finished running
  if(threadsRunning > 0) {
    printf("\nSHUTDOWN AFTER ALL THREADS COMPLETED\n");
    // if the user presses ^C twice, ungracefully exit
    printf("\nPRESS ^C AGAIN TO COMPLETELY TERMINATE EXECUTION\n");
    while(threadsRunning > 0);
    printf("Threads finished running\n");
  }
  printf("Threads Made: %d: \n", threadsMade);
  double avgTime = fileAccessTime / (double) threadsMade;
  printf("Average Thread Time: %f seconds\n", avgTime);
  exit(1);
}

