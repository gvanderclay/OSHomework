// header file to generate key
#ifndef _sharedMem_h
#define _sharedMem_h
#include <sys/types.h>

#define SHARED_MEM_SIZE 1028
#define QUIT_VAL "quit\n"
#define true 1
#define false 0

void generate_key(key_t *);

int generate_id(key_t, int);

struct message {
  int readStatus;
  int writeStatus;
  char message[SHARED_MEM_SIZE];
};

#endif
