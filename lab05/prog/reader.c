#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sharedMem.h"


int main() {
  key_t * shmKey = (key_t *) malloc(sizeof(key_t *));
  int shmId;
  struct message * shmPtr;
  char inputBuffer[SHARED_MEM_SIZE];

  generate_key(shmKey);
  shmId = generate_id(*shmKey, IPC_CREAT | S_IRUSR);
  printf("SHMKEY %d\n", *shmKey);
  printf("SHMID %d\n", shmId);

  printf("Attaching memory\n");
  if((shmPtr = (struct message *) shmat(shmId, NULL, 0)) < 0) {
    perror("can't attach");
    exit(1);
  }


  printf("Awaiting input\n");
  while(true) {
    while(shmPtr->writeStatus == false);
    strcpy(inputBuffer, shmPtr->message);
    printf("Received %s\n", inputBuffer);
    shmPtr->readStatus--;
    if(!strcmp(inputBuffer, QUIT_VAL)) {
      printf("Quiting Reader\n");
      break;
    }
    while(shmPtr->writeStatus == true);
  }

  // free unused stuff after it's done 
  printf("Detaching memory\n");
  if(shmdt(shmPtr) < 0) {
    perror("can't detach");
    exit(1);
  }
  /* shmctl(shmId, IPC_RMID, NULL); */
  free(shmKey);
}

