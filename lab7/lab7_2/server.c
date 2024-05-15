#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define BUFFER_SIZE 1024
#define PORT 8888

int sockfd;

void HandleError(char* message){
  perror(message);
  exit(EXIT_FAILURE);
}

void HandleClient(int sockfd, struct sockaddr_in* client_addr){
  char buffer[BUFFER_SIZE];
  int read_size;
  printf("Client connected: %s:%d, handling session in PID %d\n",
           inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), getpid());
  while ((read_size = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
    send(sockfd, buffer, read_size, 0);
    printf("Echoed back: %.*s\n", read_size, buffer);
  }
  if (read_size == 0) {
    puts("Client disconnected");
  }
  else if (read_size == -1) {
    perror("recv failed");
  }
  if(close(sockfd) == -1) HandleError("Close fail");
  exit(0);
}

void SignalHandler(int sig){
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

void ExitHandler(int sig){
  if(close(sockfd) == -1) HandleError("Close fail");
  exit(0);
}

int main(){
  signal(SIGCHLD, SignalHandler);
  signal(SIGINT, ExitHandler);
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
  while(1){
    if((connection_sockfd = accept(sockfd, (struct sockaddr*)&client, &client_length)) == -1){
      if(close(sockfd) == -1) HandleError("Close fail");
      HandleError("Accept fail");
    }
    if(connection_sockfd == -1){
      perror("Connection fail");
      continue;
    }
    int f = fork();
    if(f == -1){
      perror("Fork fail");
      continue;
    }
    else if(f == 0){
      if(close(sockfd) == -1) HandleError("Close fail");
      HandleClient(connection_sockfd, &client);
    }
    if(close(connection_sockfd) == -1) HandleError("Close fail");
  }
  if(close(sockfd) == -1) HandleError("Close fail");
  return 0;
}
