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
    sleep(20); //time to watch /proc/pid/maps
    exit(5);
  }
  else{
    printf("local %d %p\n", local, &local);
    printf("global %d %p\n", global, &global);
    sleep(30);
    printf("local %d %p\n", local, &local);
    printf("global %d %p\n", global, &global);
    int status;
    waitpid(npid, &status, 0);
    if(WIFEXITED(status)){
      printf("Child process terminated normally with exit code %d\n", WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
      printf("Child process terminated by signal %d\n", WTERMSIG(status));
    }
    else if(WIFSTOPPED(status)){
      printf("Child process stopped by signal %d\n", WSTOPSIG(status));
    }
    else{
      printf("Unknown termination reason\n");
    }
  }
  return 0;
}
