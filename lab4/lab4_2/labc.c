#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <time.h>

void StackArrayFunc(){
  char array[4096];
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 100000000;
  nanosleep(&ts, NULL);
  StackArrayFunc();
}

void HeapAlloc(){
  char* arr_of_allocs[100];
  if(arr_of_allocs == NULL) perror("error");
  int i = 0;
  for(i; i < 100; i++){
    arr_of_allocs[i] = malloc(4096);
    if(arr_of_allocs[i] == NULL){
      perror("error");
      i--;
      break;
    }
    sleep(1);
  }
  for(int j = 0; j <= i; j++){
    free(arr_of_allocs[j]);
  }
}

void FuncWithMMAP(){
  int pagesize = 4096;
  int blocksize = pagesize * 10;
  void* block = mmap(NULL, blocksize, PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if(block == MAP_FAILED){
    perror("error");
    exit(1);
  }
  if(mprotect(block, blocksize, PROT_NONE));
  //int a = *(int*)block;
  //*(int*)block = 1;
  if (munmap(block+pagesize*4, pagesize*2) == -1) {
    perror("error");
  }
  sleep(5);
  if (munmap(block+pagesize*6, pagesize*4) == -1) {
    perror("error");
  }
  if (munmap(block, pagesize*4) == -1) {
    perror("error");
  }
  //if (munmap(block, pagesize*10) == -1) {
  //  perror("error");
  //}
}

void SignalHandler(int signal){
  printf("got signal\n");
}

int main(int argc, char** argv){
  if(signal(SIGSEGV, SignalHandler) == SIG_ERR){
    printf("signal setting error");
    return 1;
  }
  printf("%d\n", getpid());
  sleep(7);
  //StackArrayFunc();
  HeapAlloc();
  //FuncWithMMAP();
  return 0;
}
