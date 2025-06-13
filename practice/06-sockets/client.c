#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
  // socket()
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    return 1;
  }

  // connect()
  printf("Connecting to localhost:8080...\n");
  struct in_addr sin_addr;
  inet_aton("127.0.0.1", &sin_addr);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  server_address.sin_addr = sin_addr;

  if (connect(sockfd, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("connect");
    return 1;
  }
  printf("Connected!\n");

  printf("Message recieved:\n");
  char buffer[BUFFER_SIZE];
  int read_status;
  while ((read_status = read(sockfd, buffer, BUFFER_SIZE)) != 0) {
    if (read_status == -1) {
      perror("read");
      return 1;
    }

    printf("%s", buffer);
  }

  printf("\n");

  return EXIT_SUCCESS;
}
