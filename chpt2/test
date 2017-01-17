#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  char buf[256];

  printf("Enter filename to transfer: ");
  if(fgets(buf, sizeof buf, stdin)){
    buf[strlen(buf) - 1] = '\0';
  }
  FILE* fromFile = fopen(buf, "r");
  if(!fromFile) {
    printf("Invalid file %s\n", buf);
    return EXIT_FAILURE;
  }
  printf("Enter filename to transfer to: ");
  if(fgets(buf, sizeof buf, stdin)){
    buf[strlen(buf) - 1] = '\0';
  }
  FILE* toFile = fopen(buf, "w");
  if(!toFile) {
    printf("Invalid file %s\n", buf);
    return EXIT_FAILURE;
  }
  char c;
  while((c = fgetc(fromFile)) != EOF) {
    putc(c, toFile);
  }

  printf("File copied Successfully");
  fclose(fromFile);
  fclose(toFile);
}
