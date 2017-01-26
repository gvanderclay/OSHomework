#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int fd_in = open("temp", O_RDWR | O_CREAT, 0644);
  int pid = fork();
    if(pid == 0){
      dup2(fd_in, STDOUT_FILENO);
      execl("/bin/ls", "ls", NULL);
    }
    else {
      printf("parent");
    }
}
