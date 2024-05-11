#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
  printf("%d\n", getpid());
  sleep(1);
  execv("laba", argv);
  printf("hello world");
  return 0;
}
