#include "sharedMem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void generate_key(key_t * key) {
  *key = ftok("makefile", 'x');
}

int generate_id(key_t key, int shmflg) {
  int shmId;
  if((shmId = shmget(key, sizeof(struct message), shmflg)) < 0) {
    if(errno == EACCES) {
      printf("Permission denied\n");
    }
    perror("Error creating shared memory id");
    exit(1);
  }
  return shmId;
}
