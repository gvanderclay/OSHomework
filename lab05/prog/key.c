#include "key.h"
#include <sys/types.h>
#include <sys/ipc.h>

void generate_key(key_t * key) {
  *key = ftok("makefile", 'x');
}
