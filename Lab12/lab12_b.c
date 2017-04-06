#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
  DIR *dirPtr;
  struct dirent *entryPtr;
  struct stat statBuf;

  dirPtr = opendir(".");

  while ((entryPtr = readdir(dirPtr))){
    char * filename = entryPtr->d_name;

    if(stat(filename, &statBuf) < 0) {
      printf("Error calling stat\n");
    }
    printf("Filename: %s, ", filename);
    printf("Size: %ld\n", statBuf.st_size);
  }
  closedir(dirPtr);
  return 0;
}
