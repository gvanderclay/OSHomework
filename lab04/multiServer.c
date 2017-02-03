#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define true 1
#define false 0
void *accessFile(void *arg);
void sleeper();

int main() {
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
    } else {
      printf("fgets error\n");
      exit(1);
    }
  }
}

void *accessFile(void *arg) {
  sleeper();
  char * fileName = (char *)arg;
  printf("%s\n", fileName);
  return NULL;
}


void sleeper(){
	int delay = rand() % 5;
	if (delay < 4) {
    sleep(1);
  }
  else{
  	delay = rand() % 4;
  	switch(delay){
  		case 0:
  			sleep(7);
  			break;
  		case 1:
  			sleep(8);
  			break;
  		case 2:
  			sleep(9);
  			break;
  		case 3:
  			sleep(10);
  			break;
  	}

  }
  return;
}
