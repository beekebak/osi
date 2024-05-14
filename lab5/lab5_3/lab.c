#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>

void SecondFunc(char* str, int depth){
  printf("%s\n", str);
  if(depth < 10){
    char new_str[] = "hello world";
    SecondFunc(new_str, depth+1);
  }
}

int FirstFunc(){
  char str[] = "hello world";
  SecondFunc(str, 0);
}

char* stack;

int main(){
  int fd = open("stack", O_RDWR | O_TRUNC);
  if(fd == -1){
    perror("couldnt open file");
    exit(1);
  }
  if(ftruncate(fd, 1024*1024) == -1){
    perror("cant make enough space in file");
    if(close(fd) == -1) perror("close");
    exit(1);
  }

  stack = mmap(NULL, 1024*1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(stack == MAP_FAILED){
    perror("mmap error");
    if(close(fd) == -1) perror("close");
    exit(1);
  }
  int pid = clone(FirstFunc, stack + 1024*1024, CLONE_FILES | CLONE_VM | SIGCHLD, NULL);
  if(pid == -1){
    perror("clone error");
    if(close(fd) == -1) perror("close");
    exit(1);
  }
  if(pid > 0){
    int status;
    if(waitpid(pid, &status, 0) == -1){
      perror("waitpid");
    }
  }
  if(munmap(stack, 1024*1024) == -1) perror("munmap");
  if(close(fd) == -1) perror("close");
  return 0;
}
