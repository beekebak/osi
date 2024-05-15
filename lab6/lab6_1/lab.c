#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <limits.h>

#define PAGE_SIZE 4096

int main(){
  unsigned int* shared_mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if(shared_mem == MAP_FAILED){
    perror("Map failure");
    return 1;
  }
  pid_t pid = fork();
  if(pid == -1){
    perror("Fork failure");
    if(munmap(shared_mem, PAGE_SIZE) == -1) perror("Munmap failure");
    return 1;
  }
  else if(pid == 0){
    for(unsigned int value = 0;value < UINT_MAX;value++){
      shared_mem[value%(PAGE_SIZE/sizeof(unsigned int))] = value;
    }
  }
  else{
    for(unsigned int index = 0; index < UINT_MAX; index++){
      unsigned int value = shared_mem[index % (PAGE_SIZE/sizeof(unsigned int))];
      if(index != value){
        printf("Sequence error excpected %u got %u\n", index, value);
      }
    }
    int status;
    if(wait(&status) == -1) perror("wait error");
    if(munmap(shared_mem, PAGE_SIZE) == -1) perror("Munmap failure");
  }
  return 0;
}
