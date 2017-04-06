#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/queue.h>

#define true 1
#define false 0

// max number of files that can be analyzed
// by the command
#define MAX_NUM_FILES 1024


int print_long = false;
int print_inode = false;

// files that have been specified to be analyzed
char * files[MAX_NUM_FILES];

// Handle args being passed to command
void read_args(int argc, char* args[]);

// Handle an argument that is prepended with '-'
void handle_flag(char * flag);

// Handle a filename
void handle_file(char * filename);

int main(int argc, char* argv[]) {
  read_args(argc, argv);
  int i = 0;
  // for all of the files specified...
  while(files[i] != NULL) {
    handle_file(files[i]);
    i++;
  }
  printf("\n");
}

void handle_file(char * filename) {
  
}

void read_args(int argc, char* args[]) {
  int i;
  // how many files have been specified by the user
  int file_index = 0;
  for(i = 1; i < argc; i++) {
    // check if the argument is a flag
    if(args[i][0] == '-') {
      handle_flag(args[i]);
    }
    else {
      files[file_index] = args[i];
      ++file_index;
    }
  }
  files[file_index] = NULL;
}

void handle_flag(char * flag) {
  int j;
  int argLength = strlen(flag);
  for(j = 1; j < argLength; j++) {
    if(flag[j] == 'l') {
      print_long = true;
    }
    else if(flag[j] == 'i') {
      print_inode = true;
    }
    else {
      printf("ls: unrecognized option '%c'\n", flag[j]);
      printf("Reminder: this isn't actually ls\n");
      exit(1);
    }
  }
}
