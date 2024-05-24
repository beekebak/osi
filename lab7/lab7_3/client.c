#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8888

void HandleError(char* message){
  perror(message);
  exit(1);
}

int main(){
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) HandleError("Couldnt create socket");
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) == -1){
    if(close(sockfd) == -1) HandleError("close socket error");
    HandleError("cant set IP address");
  }
  if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1){
    if(close(sockfd) == -1) HandleError("close socket error");
    HandleError("cant connect");
  }
  for(int i = 0; i < 5; i++){
    char msg[BUFFER_SIZE] = "hello server\n";
    if(send(sockfd, msg, strlen(msg), 0) == -1){
      if(close(sockfd) == -1) HandleError("close socket error");
      HandleError("cant send msg");
    }
    if(recv(sockfd, msg, BUFFER_SIZE, 0) == -1){
      if(close(sockfd) == -1) HandleError("close socket error");
      HandleError("cant recieve msg");
    }
    printf("%s\n", msg);
  }
  if(close(sockfd) == -1) HandleError("close socket error");
  return 0;
}
