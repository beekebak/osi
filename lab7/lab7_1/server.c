#include <stdio.h>
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
  exit(EXIT_FAILURE);
}

int main(){
  struct sockaddr_in server;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd == -1) HandleError("Couldnt create socket");
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1){
    if(close(sockfd) == -1) HandleError("Close fail");
    HandleError("Bind fail");
  }
  struct sockaddr_in client;
  socklen_t client_length = sizeof(client);
  char buffer[BUFFER_SIZE];
  while(1){
    int ret = recvfrom(sockfd, buffer, BUFFER_SIZE, 0 , (struct sockaddr*)&client, &client_length);
    if(ret == -1){
      if(close(sockfd) == -1) HandleError("Close fail");
      HandleError("recv fail");
    }
    printf("Recieved %s\n", buffer);
    ret = sendto(sockfd, buffer, ret, 0, (struct sockaddr*)&client, client_length);
    if(ret == -1){
      if(close(sockfd) == -1) HandleError("Close fail");
      HandleError("send fail");
    }
  }
  if(close(sockfd) == -1) HandleError("Close fail");
  return 0;
}
