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
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockfd < 0) HandleError("Couldnt create socket");
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) == -1){
    if(close(sockfd) == -1) HandleError("close socket error");
    HandleError("cant set IP address");
  }
  char msg[BUFFER_SIZE] = "Hello server\n";
  int ret = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&server, sizeof(server));
  if(ret < 0){
    if(close(sockfd) == -1) HandleError("close socket error");
    HandleError("cant set IP address");
  }
  char input[BUFFER_SIZE];
  ret = recvfrom(sockfd, input, BUFFER_SIZE, 0, NULL, NULL);
  if(ret < 0){
    if(close(sockfd) == -1) HandleError("close socket error");
    HandleError("cant set IP address");
  }
  printf("Recieved %s\n", input);
  if(close(sockfd) == -1) HandleError("close socket error");
  return 0;
}
