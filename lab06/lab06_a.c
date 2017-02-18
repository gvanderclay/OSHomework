#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define SIZE 16

int main (int argc, char **argv) {
    int status;
    long int i, loop, temp, *shmPtr;
    int shmId;
    pid_t pid;
    key_t key;
    int semid;

    key = ftok(".", 'E');
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      perror("Can't get sem id");
      exit(1);
    }





    /*
     * TODO: get value of loop variable(from command - line
     * argument
     */
    loop = atoi(argv[1]);
    if ((shmId = shmget (IPC_PRIVATE, SIZE,
                         IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void *) -1) {
        perror ("can't attach\n");
        exit (1);
    }

    shmPtr[0] = 0;
    shmPtr[1] = 1;

    union semun {
      int val;
      struct semid_ds *buf;
      ushort * array;
    } argument;

    argument.val = 1;

    if(semctl(semid, 0, SETVAL, argument) < 0) {
      perror("Cannot set semaphor value");
    }

    struct sembuf waitBuf = {0, -1, 0};
    struct sembuf signalBuf = {0, 1,  0};
    if (!(pid = fork ())) {
        for (i = 0; i < loop; i++) {

            /*
             * TODO: swap the contents of shmPtr[0] and  shmPtr[1]
             */
          if((semop(semid, &waitBuf, 1)) == -1) {
            perror("Error locking sem in parent\n");
          }
          temp = shmPtr[0];
          shmPtr[0] = shmPtr[1];
          shmPtr[1] = temp;
          if((semop(semid, &signalBuf, 1)) == -1) {
            perror("Error unlocking sem in parent\n");
          }
        }
        if (shmdt (shmPtr) < 0) {
            perror ("just can 't let go\n");
            exit (1);
        }
        exit (0);
    }
    else {
        for (i = 0; i < loop; i++) {


            /*
             * TODO: swap the contents of shmPtr[1] and shmPtr[0]
             */
          if((semop(semid, &waitBuf, 1)) == -1) {
            perror("Error locking sem in parent\n");
          }
          temp = shmPtr[1];
          shmPtr[1] = shmPtr[0];
          shmPtr[0] = temp;
          if((semop(semid, &signalBuf, 1)) == -1) {
            perror("Error unlocking sem in parent\n");
          }
        }
    }

    wait (&status);
    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("just can't let go\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit (1);
    }
    if(semctl(semid, 0, IPC_RMID) < 0) {
      perror("Can't remove semaphore");
      exit(1);
    }
    return 0;
}
