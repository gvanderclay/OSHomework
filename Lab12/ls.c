#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <grp.h>
#include <time.h>
#include <libgen.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

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

// Handle a directories and files
void handle_file(char * filename);

void handle_working_directory();

void handle_directory(char * dirname);

void get_abs_path(char * dest, char * path, char * filename);

// if the given filename is a directory
int is_directory(char * filename);

void print_group(struct stat * stat_buff);

void print_time(time_t);

int main(int argc, char* argv[]) {
  read_args(argc, argv);
  int i = 0;
  // for all of the files specified...
  int length = sizeof(files) / sizeof(char *);
  while(files[i] != NULL) {
    if(is_directory(files[i])){
      handle_directory(files[i]);
    } else {
      handle_file(files[i]);
    }
    i++;
  }
  // if no files were passed to the command
  if(i == 0) {
    // handle the working directory
    handle_working_directory();
  }
  if(!print_long) {
    printf("\n");
  }
}

void handle_file(char * filename) {
  struct stat stat_buff;
  if(stat(filename, &stat_buff) < 0) {
    printf("ls: cannot access %s: No such file or directory", filename);
    return;
  }
  if(S_ISDIR(stat_buff.st_mode)) {

  }
  if(print_inode) {
    printf("%8lu ", stat_buff.st_ino);
  }
  if(print_long){
    printf((S_ISDIR(stat_buff.st_mode)) ? "d" : "-");
    printf((stat_buff.st_mode & S_IRUSR) ? "r" : "-");
    printf((stat_buff.st_mode & S_IWUSR) ? "w" : "-");
    printf((stat_buff.st_mode & S_IXUSR) ? "x" : "-");
    printf((stat_buff.st_mode & S_IRGRP) ? "r" : "-");
    printf((stat_buff.st_mode & S_IWGRP) ? "w" : "-");
    printf((stat_buff.st_mode & S_IXGRP) ? "x" : "-");
    printf((stat_buff.st_mode & S_IROTH) ? "r" : "-");
    printf((stat_buff.st_mode & S_IWOTH) ? "w" : "-");
    printf((stat_buff.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %lu", stat_buff.st_nlink);
    char * uname = malloc(sizeof(char *) * 1024);
    getlogin_r(uname, 1024);
    printf(" %s", uname);
    free(uname);
    print_group(&stat_buff);
    printf(" %d", stat_buff.st_size);
    print_time(stat_buff.st_mtime);
  }
  printf("%s  ", basename(filename));
  if(print_long) {
    printf("\n");
  }
}

void print_time(time_t time) {
  char buff[20];
  struct tm * timeinfo = localtime(&time);
  strftime(buff, sizeof(buff), "%b %d %H:%M", timeinfo);
  printf(" %s ", buff);
}

void print_group(struct stat *stat_buff) {
  struct group *grp;
  grp = getgrgid(stat_buff->st_gid);
  if(grp) {
    printf(" %s", grp->gr_name);
  } else {
    printf(" users");
  }
}

void handle_directory(char * dirname) {
  DIR *dir_ptr;
  struct dirent * entry_ptr;
  dir_ptr = opendir(dirname);
  while((entry_ptr = readdir(dir_ptr))) {
    char * filename = entry_ptr->d_name;
    // don't handle the working directory recursively
    // or the parent directory
    if((strcmp(filename, ".") != 0 &&
        strcmp(filename, "..")) != 0) {
      int len = strlen(dirname);
      // +2 because of extra / and \0
      char * fullpath = malloc(len + strlen(filename) + 2);
      get_abs_path(fullpath, dirname, filename);
      handle_file(fullpath);
      free(fullpath);
    }
  }
}

void get_abs_path(char * dest, char * path, char * filename) {
  if(dest == NULL) {
    printf("Didn't specify dest\n");
    exit(1);
  }
  sprintf(dest, "%s/%s", path, filename);
}

void handle_working_directory() {
  DIR *dir_ptr;
  struct dirent * entry_ptr;
  dir_ptr = opendir(".");
  while((entry_ptr = readdir(dir_ptr))) {
    char * filename = entry_ptr->d_name;
    // don't handle the working directory recursively
    // or the parent directory
    if((strcmp(filename, ".") != 0 &&
        strcmp(filename, "..")) != 0) {
      handle_file(entry_ptr->d_name);
    }
  }
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

int is_directory(char * filename) {
  struct stat stat_buff;
  if(stat(filename, &stat_buff) < 0) {
    printf("Error checking if is_directory\n");
    exit(1);
  }
  return S_ISDIR(stat_buff.st_mode);
}
