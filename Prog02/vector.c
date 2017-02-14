#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>

#define READ 0
#define WRITE 1

#define MAX_BUFF_SIZE 65

void continueExecution(int);

char * getInput();

FILE * readFile(char *);

void complementer(int);

void incrementer(int, int);

void adder(int);

char * add(char *, char *);

void flip(char *);

void increment(char *);

void printLog(const char *, ...);

char * inputA = "8-input_A.dat";

char * inputB = "8-input_B.dat";

int main() {
  signal(SIGINT, continueExecution);
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
      exit(1);
    }
  }

  if(complementerPID != 0 && incrementerPID != 0) {
    printLog("Spawning adder process....\n");
    if((adderPID = fork()) == -1) {
      printLog("Error forking adder");
      exit(1);
    }
  }

  if(complementerPID == 0) {
    printLog("Complementer spawned\n");
    // close the unused pipes for this process
    printLog("Closing pipes for complementer\n");
    close(ItoA[READ]);
    close(ItoA[WRITE]);

    printLog("Running complementer function\n");
    complementer(CtoI[WRITE]);
  }
  if(incrementerPID == 0 && complementerPID != 0) {
    printLog("Incrementer spawned\n");
    // close unused pipes
    printLog("Closing pipes for incrementer\n");
    close(CtoI[WRITE]);
    close(ItoA[READ]);

    printLog("Running incrementer function\n");
    incrementer(CtoI[READ], ItoA[WRITE]);
  }
  if(adderPID == 0 && complementerPID != 0 && incrementerPID != 0) {
    printLog("Adder spawned\n");
    printLog("Closing pipes for adder\n");
    close(CtoI[WRITE]);
    close(CtoI[READ]);
    close(ItoA[WRITE]);
    adder(ItoA[READ]);
    // TODO implement adder function
  }

  if(complementerPID != 0 && incrementerPID != 0 && adderPID != 0) {
    printLog("Closing pipes in main process\n");
    close(CtoI[READ]);
    close(CtoI[WRITE]);
    close(ItoA[READ]);
    close(ItoA[WRITE]);
    int status;
    // wait for all child processes to end
    pid_t wpid;
    printLog("Waiting for child processes to end\n");
    while((wpid = wait(&status)) > 0) {
      printf("PID %d: Exit status was %d\n", wpid, status);
    }
  }
}

/**
 * Pulls binary from file, flips the numbers, then
 * streams it to a pipe
 **/
void complementer(int pipe) {
  /* printLog("Complementer paused\n"); */
  /* pause(); */
  FILE * input = readFile(inputB);

  //TODO pause and catch for Ctrl-C
  char buff[MAX_BUFF_SIZE];
  while(fgets(buff, MAX_BUFF_SIZE, input)) {
    // remove newline from end of string
    buff[strcspn(buff, "\n")] = '\0';
    printLog("Complementer read in %s\n", buff);

    // flip each digit in the number
    for(int i = 0; i < strlen(buff) - 1; i++) {
      flip(&buff[i]);
    }
    printLog("Complementer sending: %s\n", buff);
    write(pipe, buff, MAX_BUFF_SIZE);
  }

  fclose(input);
  printLog("Complementer closing output pipe\n");
  close(pipe);

  printLog("Complementer function finished\n");
  exit(0);
}

/**
 * Increment the binary numbers received from the pipe,
 * then streams them to the outPipe
 **/
void incrementer(int inPipe, int outPipe) {
  char buff[MAX_BUFF_SIZE];
  printLog("Incrementer waiting for input\n");
  while(read(inPipe, buff, MAX_BUFF_SIZE)) {
    printLog("Incrementer received %s\n", buff);
    increment(buff);
    printLog("Incrementer sending %s\n", buff);
    write(outPipe, buff, MAX_BUFF_SIZE);
  }
  close(inPipe);
  close(outPipe);
  printLog("Incrementer function finished\n");
}

void adder(int inPipe) {
  char pipeBuff[MAX_BUFF_SIZE];
  char fileBuff[MAX_BUFF_SIZE];
  FILE * inputFile = readFile(inputA);

  while(read(inPipe, pipeBuff, MAX_BUFF_SIZE)) {
    fgets(fileBuff, MAX_BUFF_SIZE, inputFile);
    fileBuff[strcspn(fileBuff, "\n")] = '\0';
    printLog("Adder read in %s\n", fileBuff);
    printLog("Adder received %s\n", pipeBuff);
    add(fileBuff, pipeBuff);
  }
  printLog("Adder closing input pipe\n");
  close(inPipe);
  fclose(inputFile);
  exit(0);
}

char * add(char * a, char * b) {
  char carry = '0';
  printf("Adding: %s + %s\n", a, b);
  printLog("length: %ld\n", strlen(a));
  for(int i = strlen(a) - 2; i >= 0; i--) {
  }
  return "a";
}

void increment(char * num) {
  printLog("Incrementing\n");
  int carry = 1;
  for(int i = strlen(num) - 2; i >= 0 && carry == 1; i--) {
    if(num[i] == '1') {
      num[i] = '0';
    }
    else {
      num[i] = '1';
      carry = 0;
    }
  }
  printLog("Done incrementing\n");
}

/**
 * Flips the binary character
 */
void flip(char * ch) {
  if(*ch == '\n' || *ch == '\0') {
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

void continueExecution(int signum) {
  signal(SIGINT, exit);
  kill(getpid(), SIGCONT);
}

/*
 *
 */
void printLog( const char* format, ... ) {
  va_list args;
  char * buffer = (char *)malloc(256);
  va_start( args, format );
  vsprintf( buffer, format, args );
  // TODO change this back to stderr
  fprintf(stderr, "PID %d: %s", getpid(), buffer);
  free(buffer);
  va_end( args );
}
