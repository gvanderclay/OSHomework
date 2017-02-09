#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>

#define READ 0
#define WRITE 1

char * getInput();

FILE * readFile(char *);

void complementer(int);

void incrementer(int, int);

void flip(int *);

char * increment(char *);

void printLog(const char *, ...);

char * inputB = "8-input_B.dat";

int main() {
  // Complementer to Incrementer;
  int CtoI[2];

  // Incrementer to Adder
  int ItoA[2];

  pid_t complementerPID;
  pid_t incrementerPID;
  pid_t adderPID;


  printLog("Creating pipes\n");
  pipe(CtoI);
  pipe(ItoA);

  printLog("Spawning complementer process....\n");
  if((complementerPID = fork()) == -1) {
    printLog("Error forking complementer\n");
    exit(1);
  }

  if(complementerPID != 0) {
    printLog("Spawning incrementer process....\n");
    if((incrementerPID = fork()) == -1) {
      printLog("Error forking incrementer");
    }
  }

  if(complementerPID != 0 && incrementerPID != 0) {
    printLog("Spawning adder process....\n");
    if((adderPID = fork()) == -1) {
      printLog("Error forking adder");
    }
  }

  if(complementerPID == 0) {
    // close the unused pipes for this process
    printLog("Closing pipes for complementer\n");
    close(ItoA[READ]);
    close(ItoA[WRITE]);

    // close the output side of the pipe used in this
    // process
    close(CtoI[READ]);
    printLog("Running complementer function\n");
    complementer(CtoI[WRITE]);
  }
  if(incrementerPID == 0) {
    // close unused pipes
    printLog("Closing pipes for incrementer\n");
    close(CtoI[WRITE]);
    close(ItoA[READ]);

    printLog("Running incrementer function\n");
    incrementer(CtoI[READ], ItoA[WRITE]);
  }
  if(adderPID == 0) {
    printLog("Closing pipes for adder\n");
    close(CtoI[WRITE]);
    close(CtoI[READ]);
    close(ItoA[WRITE]);
    // TODO implement adder function
  }

  if(complementerPID != 0 && incrementerPID != 0 && adderPID != 0) {
    close(CtoI[READ]);
    close(CtoI[WRITE]);
    close(ItoA[READ]);
    close(ItoA[WRITE]);
    // wait for all child processes to end
    while(waitpid(-1, NULL, 0)) {
      if(errno == ECHILD) {
        break;
      }
    }
  }
}

/**
 * Pulls binary from file, flips the numbers, then
 * streams it to a pipe
 **/
void complementer(int pipe) {
  FILE * input = readFile(inputB);

  //TODO pause and catch for Ctrl-C
  int ch;
  while((ch = fgetc(input)) != EOF) {
    flip(&ch);
    char buff = ch;
    if(ch == '\n') {
      printLog("Complementer sending: \\n\n");
    }
    else {
      printLog("Complementer sending: %c\n", buff);
    }
    write(pipe, &buff, 1);
  }

  fclose(input);

  printLog("Complementer function finished\n");
  exit(0);
}

/**
 * Increment the binary numbers received from the pipe,
 * then streams them to the outPipe
 * TODO actually increment each binary number
 **/
void incrementer(int inPipe, int outPipe) {
  char ch;

  while(read(inPipe, &ch, 1) == 1) {
    if(ch == '\n') {
      printLog("Incrementer receiving: \\n\n");
    }
    else {
    printLog("Complementer receiving: %c\n", ch);
    }
  }

  printLog("Incrementer function finished\n");
}

/**
 * Flips the binary character
 */
void flip(int * ch) {
  if(*ch == '\n') {
    return;
  }

  *ch = *ch == '1' ? '0': '1';
}


FILE * readFile(char * fileName) {
  printLog("File %s created\n", fileName);
  FILE * file;
  file = fopen(fileName, "r");
  if(file == NULL) {
    printLog("Can't open input file %s\n", fileName);
    exit(1);
  }
  return file;
}

/*
 *
 */
void printLog( const char* format, ... ) {
  va_list args;
  char * buffer = (char *)malloc(256);
  va_start( args, format );
  vsprintf( buffer, format, args );
  fprintf(stderr, "PID %d: %s", getpid(), buffer);
  free(buffer);
  va_end( args );
}
