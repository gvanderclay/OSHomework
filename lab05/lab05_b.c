#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FOO 4096

/**
 * modified version of lab05_a.c
 * Used to answer problem 4 of lab 5 in CIS 452
 */
int main () {
  int shmId;
  char *shmPtr;
  struct shmid_ds * buf = (struct shmid_ds *)malloc(sizeof(struct shmid_ds));

  if ((shmId =
       shmget (IPC_PRIVATE, FOO,
               IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
    perror ("i can't get no..\n");
    exit (1);
  }
  if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
    perror ("can't attach\n");
    exit (1);
  }
  printf ("%d\n", shmId);
  pause();
  if (shmdt (shmPtr) < 0) {
    perror ("just can't let go\n");
    exit (1);
  }
  if (shmctl (shmId, IPC_STAT, buf) < 0) {
    perror ("can't get shared memory info\n");
    exit (1);
  }
  if (shmctl (shmId, IPC_RMID, 0) < 0) {
    perror ("can't deallocate\n");
    exit (1);
  }
  size_t size = buf->shm_segsz;

  printf("%ld\n", size);
  free(buf);


  return 0;
}
