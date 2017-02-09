#include <stdio.h>
#include <stdlib.h>

char * getInput();

FILE * readFile(char *);

void complementer();

void flip(char *);

char * increment(char *);

char * inputB = "8-input_B.dat";

int main() {
  complementer();
}

/**
 * Pulls binary from file, flips the numbers, then
 * streams it to a pipe
 **/
void complementer() {
  char ch;
  FILE * input = readFile(inputB);
  printf("%s\n", inputB);

  while((ch = fgetc(input) != EOF)) {
    flip(&ch);
    printf("%c", ch);
  }
  fclose(input);
}

void flip(char * ch) {
  if(*ch == '\n') {
    return;
  }

  *ch = *ch == '1' ? '0': '1';
}

FILE * readFile(char * fileName) {
  FILE * file;
  file = fopen(fileName, "r");
  if(file == NULL) {
    fprintf(stderr, "Can't open input file %s\n", fileName);
    exit(1);
  }
  return file;
}

