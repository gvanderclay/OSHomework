#include <sys/sem.h>

int main() {
  key_t key;
  int semid;

  key = ftok(".", 'E');
  semid = semget(key, 1, 0666 | IPC_CREAT);

  semctl(semid, 0, IPC_RMID);
}
