#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  FILE* f = fopen("file", "r");
  if(f == NULL){
    perror("couldnt open file");
    exit(1);
  }
  char file_containment[4096];
  fread(file_containment, 4096, 4096, f);
  setuid(getuid());
  printf("%s", file_containment);
  int uid = getuid();
  int euid = geteuid();
  printf("%d\n", uid);
  printf("%d\n", euid);
}
