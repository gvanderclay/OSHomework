#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]) {


  if(argc < 4) {
    printf("Input format: linkType linkSrc linkDest\n");
    exit(1);
  }

  if(strcmp(argv[1], "hard") == 0){
    if(link(argv[2], argv[3]) < 0) {
      perror("Error creating hard link");
      exit(1);
    }
    printf("Hard link from %s to %s successfully made", argv[2], argv[3]);
    exit(0);
  } else if(strcmp(argv[1], "soft") == 0) {
    if(symlink(argv[2], argv[3]) < 0) {
      perror("Error creating soft link");
      exit(1);
    }
    printf("Soft link from %s to %s successfully made", argv[2], argv[3]);
    exit(0);
  } else {
    printf("Invalid linkType %s: use hard or soft", argv[1]);
    exit(1);
  }

  return 0;

}
