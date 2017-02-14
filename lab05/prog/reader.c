#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "key.h"


int main() {
  key_t * readerKey = (key_t *) malloc(sizeof(key_t *));
  generate_key(readerKey);
  printf("%d\n", *readerKey);
  free(readerKey);
}

