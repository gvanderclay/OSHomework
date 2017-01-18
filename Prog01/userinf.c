/*
 * Author: Gage Vander Clay
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

void printHostName();

void printPasswordFileEntries();

void getEnvironmentVar();


int main() {
  printf("User is: %s\n", getlogin());

  printf("UID is: %d\n", getuid());

  printf("GID is: %d\n", getgid());

  printHostName();

  printPasswordFileEntries();

  printf("\n");

  getEnvironmentVar();

}

void printHostName() {
  int bufLen = 128;
  char * buff = (char *) malloc(bufLen);

  gethostname(buff, bufLen);
  printf("hostname is %s\n", buff);
  free(buff);
}

void printPasswordFileEntries() {
  int i = 0;
  for(; i < 5; i++) {
    struct passwd * pwent = getpwent();
    printf("entry %d: %s\n", i, pwent->pw_name);
  }
  endpwent();
}

void getEnvironmentVar() {
  printf("environment variable desired? ");
  char input[128];

  fgets(input, 128, stdin);
}
