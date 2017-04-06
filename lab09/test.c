#include <stdlib.h>
#include <string.h>
#include<stdio.h>

char * global_init_string = "Sup dawg";

char global_init_string_1[] = "Sup man";

int global = 0;

int uninitialized_global;


struct test {
  char a;
  int b;
};

int stack_heap_function() {
  global++;
  int * a = (int *)malloc(sizeof(int));
  int b = 0;
  printf("a begins at %p\n", a);
  printf("b begins at %p\n", &b);
  if(global == 10) {
    return 0;
  }
  stack_heap_function();
  free(a);
  return *a;
}

int test_func() {
  printf("ayyo waddup");
  return 0;
}


int main() {
  int a = global + 1;
  int uninitialized_local;
  char * local_init_string = "Sup dawgs";
  struct test test_struct = {'a', 1};

  stack_heap_function();

  printf("global_init_string addresses at:\n");
  for (int i = 0; i < strlen(global_init_string); i++) {
    printf("-- %p\n", &global_init_string[i]);
  }

  printf("global_init_string_1 addresses at:\n");
  for (int i = 0; i < strlen(global_init_string_1); i++) {
    printf("-- %p\n", &global_init_string_1[i]);
  }

  printf("local_init_string addresses at:\n");
  for (int i = 0; i < strlen(local_init_string); i++) {
    printf("-- %p\n", &local_init_string[i]);
  }

  printf("global is at %p\n", &global);
  printf("uninitialized_global is at %p\n", &uninitialized_global);
  printf("uninitialized_local is at %p\n", &uninitialized_local);
  printf("stack_heap_function is at %p\n", &stack_heap_function);
  printf("test_func is at %p\n", &test_func);
  printf("main is at %p\n", &main);
  printf("test struct is at %p\n", &test_struct);
  printf("test_struct->a is at %p\n", &(test_struct.a));
  printf("test_struct->b is at %p\n", &(test_struct.b));
  return a;
}


