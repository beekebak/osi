#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <limits.h>

#define PAGE_SIZE 4096

int main(){
  int pipefd[2];
  if(pipe(pipefd) == -1){
    perror("Couldnt initialize pipe");
    return 1;
  }
  pid_t pid = fork();
  if(pid == -1){
    perror("Fork failure");
    if(close(pipefd[0]) == -1) ("Pipe close error");
    if(close(pipefd[1]) == -1) ("Pipe close error");
    return 1;
  }
  else if(pid == 0){
    if(close(pipefd[0]) == -1) ("Pipe close error");
    for(unsigned int value = 0; value < UINT_MAX; value++){
      if(write(pipefd[1], &value, sizeof(unsigned int)) == -1) return 1;
    }
    if(close(pipefd[1]) == -1) ("Pipe close error");
    return 0;
  }
  else{
    if(close(pipefd[1]) == -1) ("Pipe close error");
    for(unsigned int index = 0; index < UINT_MAX; index++){
      unsigned int value;
      read(pipefd[0], &value, sizeof(unsigned int));
      if(index != value){
        printf("Sequence error excpected %u got %u\n", index, value);
      }
    }
    if(close(pipefd[0]) == -1) ("Pipe close error");
    int status;
    if(wait(&status) == -1) perror("wait error");
  }
  return 0;
}
