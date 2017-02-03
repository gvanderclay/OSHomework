#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

#define true 1
#define false 0
void * accessFile(void *arg);


int main() {
  char buffer[256];
  while(true) {
    printf("Enter a filename: ");
    if(fgets(buffer, sizeof(buffer), stdin)) {
      // remove newline from end of string
      size_t ln = strlen(buffer) - 1;
      if(buffer[ln] == '\n') {
        buffer[ln] = '\0';
      }
      printf("%s\n", buffer);
    }
    else {
      printf("fgets error\n");
      exit(1);
    }
  }

void* accessFile(void *arg){
	int delay = rand(time(NULL))%4;
	if(delay < 3){
		sleep(1);
	}
}
