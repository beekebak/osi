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
  printf("%s", file_containment);
  gid_t uid = getuid();
  gid_t euid = geteuid();
  printf("%d\n", uid);
  printf("%d\n", euid);
}