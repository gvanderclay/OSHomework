#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 30

int main (int argc, char *argv[]) {
  struct flock fileLock;
  int fd;
  char buff[SIZE];

  if (argc < 2) {
    printf ("usage: filename\n");
    exit (1);
  }
  if ((fd = open (argv[1], O_RDWR)) < 0) {
    perror ("there is");
    exit (1);
  }

  fileLock.l_type = F_RDLCK;
  fileLock.l_whence = SEEK_SET;
  fileLock.l_start = 0;
  fileLock.l_len = 0;
  if(fcntl(fd, F_SETLKW, &fileLock)) {
    perror("no way");
    exit(1);
  }

  read(fd, buff, SIZE - 2);
  if(fcntl(fd, F_UNLCK, &fileLock)) {
    perror("Error unlocking");
    exit(1);
  }

  printf("Output: %s", buff);

  close(fd);
  return 0;
}
