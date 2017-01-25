#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
  pid_t pid, pid1;
  pid = fork();
  if (pid < 0) {
    fprintf (stderr, "fork() failed\n");
    return(1);
  }
  else if (pid == 0) {
    printf("child\n");
    pid1 = getpid();
    printf ("pid = %d\n", pid); // A
    printf ("pid1 = %d\n", pid1); // B
  }
  else {
    wait(NULL);
    printf("parent\n");
    pid1 = getpid();
    printf ("pid = %d\n", pid); // C
    printf ("pid1 = %d\n", pid1); // D
  }
  return 0;
}
