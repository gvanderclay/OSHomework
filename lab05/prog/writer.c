#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
  shmId = generate_id(*shmKey, IPC_CREAT | S_IWUSR | S_IRUSR);
  printf("SHMID %d\n", shmId);


  printf("Attaching memory\n");
  if((shmPtr = (struct message *) shmat(shmId, NULL, 0)) < 0) {
    perror("can't attach");
    exit(1);
  }

  shmPtr->readStatus = 2;
  shmPtr->writeStatus = false;


  while(true) {
    printf("Enter data to be written: ");
    fgets(inputBuffer, SHARED_MEM_SIZE, stdin);
    strcpy(shmPtr->message, inputBuffer);
    shmPtr->writeStatus = true;
    printf("Waiting for readers\n");
    while(shmPtr->readStatus > 0);
    shmPtr->readStatus = 2;
    shmPtr->writeStatus = false;
    if(!strcmp(inputBuffer, QUIT_VAL)) {
      printf("Quitting writer\n");
      break;
    }
  }


  // free unused stuff after it's done 
  printf("Detaching memory\n");
  if(shmdt(shmPtr) < 0) {
    perror("can't detach");
    exit(1);
  }
  shmctl(shmId, IPC_RMID, NULL);
  free(shmKey);
}
