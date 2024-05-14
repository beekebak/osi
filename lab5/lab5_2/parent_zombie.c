#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global = 2;

int main(){
  int local = 1;
  printf("local %d %p\n", local, &local);
  printf("global %d %p\n", global, &global);
  printf("pid %d\n", getpid());
  int npid = fork();
  if(npid == -1){
    perror("error\n");
    exit(1);
  }
  if(npid == 0){
    printf("pid %d, ppid %d\n", getpid(), getppid());
    local = 8;
    global = 13;
    printf("local %d %p\n", local, &local);
    printf("global %d %p\n", global, &global);
    sleep(20); //time to wait parent process end
    exit(5);
  }
  return 0;
}
