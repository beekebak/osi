#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>

typedef struct List{
  struct List* next;
  struct List* prev;
  int size;
} List;

List* head;
List* tail;

char* heap;

void* Mylloc(int size){
  int realsize = size+sizeof(List);
  List* iter = head;
  while(iter->next != head){
    if(((char*)iter->next - ((char*)iter + iter->size + sizeof(List))) >= realsize){
      List* cur = (List*)(((char*)iter)+iter->size+sizeof(List));
      cur->size = size;
      cur->next = iter->next;
      cur->prev = iter;
      iter->next->prev = cur;
      iter->next = cur;
      return (void*)((char*)cur+sizeof(List));
    }
    iter = iter->next;
  }
  return NULL;
}

void Mfree(void* ptr){
  List* cur = (List*)(((char*)ptr) - sizeof(List));
  cur->prev->next = cur->next;
  cur->next->prev = cur->prev;
}

int main(){
  int fd = open("file", O_RDWR | O_TRUNC);
  if(fd == -1){
    perror("couldnt open file");
    exit(1);
  }
  if(ftruncate(fd, 1024*4) == -1){
    perror("cant make enough space in file");
    if(close(fd) == -1) perror("close");
    exit(1);
  }
  heap = mmap(NULL, 1024*4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(heap == MAP_FAILED){
    perror("mmap error");
    if(close(fd) == -1) perror("close");
    exit(1);
  }
  head = (List*) heap;
  tail = (List*) (heap + 4096 - 1 - sizeof(List));
  printf("%p %p\n", head, tail);
  head->next = tail;
  head->prev = tail;
  tail->next = head;
  tail->prev = head;
  head->size = 0;
  tail->size = 0;
  sleep(5);
  void* reg = Mylloc(100);
  sleep(5);
  void* reg2 = Mylloc(1000);
  sleep(5);
  Mfree(reg);
  void* reg3 = Mylloc(20);
  void* reg4 = Mylloc(25);
  void* reg5 = Mylloc(5000);
  assert(reg5 == NULL);
  //test
  if(munmap(heap, 1024*4) == -1) perror("munmap");
  if(close(fd) == -1) perror("close");
  return 0;
}
