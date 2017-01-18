/*
 * Author: Gage Vander Clay
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

void printHostName();

void printPasswordFileEntries();

void getEnvironmentVar();

char * trimNewLine(char *);

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
  // get the first 5 entries from the /etc/passwd file
  for(; i < 5; i++) {
    struct passwd * pwent = getpwent();
    printf("entry %d: %s\n", i, pwent->pw_name);
  }
  endpwent();
}

void getEnvironmentVar() {
  printf("environment variable desired? ");
  int bufLen = 128;
  char * input = (char *) malloc(bufLen);

  fgets(input, 128, stdin);
  input = trimNewLine(input);
  char * envVar = getenv(input);
  if(envVar == NULL) {
    printf("%s is not a declared environment variable\n", input);
  } else {
    printf("value is: %s\n", envVar);
  }
}

/*
 * Need to trim the newline character from the end of
 * input since fgets doesn't do that
 */
char * trimNewLine(char * s) {
  int i = strlen(s) - 1;
  if ((i > 0) && (s[i] == '\n')) {
    s[i] = '\0';
  }
  return s;
}

