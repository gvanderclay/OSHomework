#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "key.h"


int main() {
  key_t * writerKey = (key_t *) malloc(sizeof(key_t *));
  generate_key(writerKey);
  printf("%d\n", *writerKey);
  free(writerKey);
}
