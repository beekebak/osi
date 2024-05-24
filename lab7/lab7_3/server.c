#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
#include <signal.h>


#define BUFFER_SIZE 1024
#define PORT 8888
#define MAX_CLIENTS 20

int sockfd;
struct pollfd fds[MAX_CLIENTS];
int nfds = 1;


void HandleError(char* message){
  perror(message);
  exit(EXIT_FAILURE);
}

void ChildHandler(int sig){
  int status;
  int child_return_code = wait(&status);
  if(child_return_code == -1) write(2, "Quit checker returned with error\n", 34);
  if(WIFEXITED(status) != 0) write(2, "Quit checker did not returned successfully\n", 44);
  else if(WEXITSTATUS(status) != 0) write(2, "Quit checker returned with wrong value\n", 40);
  for(int i = 0; i < nfds; i++){
    if(close(fds[i].fd) == -1) write(2, "Close fail fd signal handler", 29);
  }
  exit(0);
}

void ExitHandler(int sig){
  for(int i = 0; i < nfds; i++){
    if(close(fds[i].fd) == -1) perror("Close fail fd signal handler");
  }
  exit(0);
}

int main(){
  signal(SIGINT, ExitHandler);
  signal(SIGCHLD, ExitHandler);
  int pid = fork();
  if(pid == 0){
    char exit_msg[4096];
    while(1){
      scanf("%s", exit_msg);
      if(strcmp(exit_msg, "quit") == 0) return 1;
    }
  }
  struct sockaddr_in server;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) HandleError("Couldnt create socket");
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1){
    HandleError("Bind fail");
  }
  if(listen(sockfd, 5) < 0){
    if(close(sockfd) == -1) HandleError("Close fail");
    HandleError("Listen fail");
  }
  struct sockaddr_in client;
  socklen_t client_length = sizeof(client);
  int connection_sockfd;
  memset(fds, 0, sizeof(fds));
  fds[0].fd = sockfd;
  fds[0].events = POLLIN;
  char buffer[BUFFER_SIZE];
  while(1){
    int ret = poll(fds, nfds, -1);
    if(ret == -1){
      perror("Poll fail");
      break;
    }
    for(int i = 0; i < nfds; i++){
      if(fds[i].revents & POLLIN){
        if(fds[i].fd == sockfd){
          int addr_len = sizeof(client);
          connection_sockfd = accept(sockfd, (struct sockaddr*)&client, &addr_len);
          if(connection_sockfd == -1){
            printf("Connection error");
            continue;
          }
          fds[nfds].fd = connection_sockfd;
          fds[nfds].events = POLLIN;
          nfds++;
        }
        else{
          ret = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
          if(ret == -1){
            perror("Recv fail");
            if(close(fds[i].fd) == -1) perror("Close fail -1");
            fds[i].fd = -1;
            continue;
          }
          else if(ret == 0){
            if(close(fds[i].fd) == -1) perror("Close fail");
            fds[i].fd = -1;
          }
          else{
            buffer[ret] = '\0';
            printf("msg from %d: %s", fds[i].fd, buffer);
            if(send(fds[i].fd, buffer, ret, 0) == -1){
              perror("Send error");
            }
          }
        }
      }
    }
    for(int i = 0; i < nfds; i++){
      if(i >= nfds) continue;
      if(fds[i].fd == -1){
        for(int j = i; j < nfds - 1; j++){
          fds[j] = fds[j+1];
        }
        i--;
        nfds--;
      }
    }
  }
  if(close(sockfd) == -1) HandleError("Close fail");
  return 0;
}
