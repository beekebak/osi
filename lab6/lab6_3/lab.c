#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <limits.h>

#define PAGE_SIZE 4096
#define SOCKET_PATH "socket"

void WaitChild(){
  int status;
  if(wait(&status) == -1) perror("wait error");
}

void PrintErrorAndClose(char* msg, int fd){
  perror(msg);
  if(close(fd) == -1) perror("Close error");
}

int ParentFunc(int fd){
  int data_socket = accept(fd, NULL, NULL);
  if(data_socket == -1){
    PrintErrorAndClose("Couldnt create socket", fd);
    WaitChild();
    return 1;
  }
  for(unsigned int index = 0; index < UINT_MAX; index++){
    unsigned int value;
    if(read(data_socket, &value, sizeof(unsigned int)) == -1){
      perror("Read error");
      WaitChild();
      return 1;
    }
    if(index != value){
      printf("Sequence error excpected %u got %u\n", index, value);
    }
  }
  WaitChild();
  return 0;
}


void ChildFunc(int fd){
  if(close(fd) == -1) perror("Close error");
  struct sockaddr_un con_addr;
  int con_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(con_socket_fd == -1){
    perror("Couldnt create socket");
    return 1;
  }
  memset(&con_addr, 0, sizeof(struct sockaddr_un));
  con_addr.sun_family = AF_UNIX;
  strncpy(con_addr.sun_path, SOCKET_PATH, sizeof(con_addr.sun_path) - 1);
  ret = connect(con_socket_fd, (struct sockaddr*)&con_addr, sizeof(addr));
  if(ret == -1){
    PrintErrorAndClose("Connection error", con_socket_fd);
    return 1;
  }
  for(unsigned int value = 0; value < UINT_MAX; value++){
    if(write(con_socket_fd, &value, sizeof(unsigned int)) == -1){
      PrintErrorAndClose("Write error", con_socket_fd);
      return 1;
    }
  }
  return 0;
}


int main(){
  struct sockaddr_un addr;
  int fd;
  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(fd == -1){
    perror("Couldnt create socket");
    exit(1);
  }
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
  int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
  if(ret == -1){
    PrintErrorAndClose("Bind error", con_socket_fd);
    return 1;
  }
  ret = listen(fd, 5);
  if(ret == -1){
    PrintErrorAndClose("Listen error", con_socket_fd);
    return 1;
  }

  pid_t pid = fork();
  if(pid == -1){
    PrintErrorAndClose("Fork failure", con_socket_fd);
    return 1;
  }
  else if(pid == 0){
    ChildFunc(fd);
  }
  else {
    if(ParentFunc == 1) break;
  }
  return 0;
}
