#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main() {
  int size = 1000000;
  int test[size];
  printf("here\n");
  clock_t begin = clock();

  for(int i = 0; i < size; i++) {
    test[i] = i;
  }
  printf("done\n");

  /* int pid = fork(); */
  /* if(pid == 0) { */
  /*   for(int i = size/2; i < size; i++) { */
  /*     test[i] = i; */
  /*   } */
  /*   exit(0); */
  /* } */
  /* else { */
  /*   int status; */
  /*   for(int i = 0; i < size/2; i++) { */
  /*     test[i] = i; */
  /*   } */
  /*   wait(&status); */
  /*   printf("child and parent done\n"); */
  /* } */

  clock_t end = clock();
  double time_spent = (double)(end - begin);
  printf("time taken: %f\n", time_spent);
}
