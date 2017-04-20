#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int randomData = open("/dev/random", O_RDONLY);

  unsigned int rand;

  int i = 0;
  while(i < 10) {
    read(randomData, &rand, sizeof(rand));
    srandom(rand);
    int rand1 = random();
    printf("%d\n", rand1);
    i++;
  }

  close(randomData);
}
