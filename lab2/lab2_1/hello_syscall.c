#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

int main(){
  char msg[] = "Hello world";
  int code = syscall(SYS_write, 1, &msg, 12);
  if(code == -1) printf("error %d", errno);
  return 0;
}
