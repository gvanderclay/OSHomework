#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
  int size = 10000000;
  int test[size];

  int pid = fork();
  if(pid == 0) {
    for(int i = size/2; i < size; i++) {
      test[i] = i;
    }
    exit(0);
  }
  else {
    for(int i = 0; i < size/2; i++) {
      test[i] = i;
    }
    wait();
    printf("child and parent done");
  }
}
