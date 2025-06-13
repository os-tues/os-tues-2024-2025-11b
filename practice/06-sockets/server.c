#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  /* sin_addr.s_addr = 0xf000001; // "127.0.0.1" */
  struct in_addr sin_addr;
  inet_aton("127.0.0.1", &sin_addr);
  server_address.sin_addr = sin_addr;

  if (bind(sockfd, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("bind");
    return 1;
  }

  if (listen(sockfd, 5) == -1) {
    perror("listen");
    return 1;
  }

  while (1) {
    printf("Waiting for a connection...\n");
    int client_fd = accept(sockfd, NULL, NULL);
    if (client_fd == -1) {
      perror("accept");
      continue; // Continue to wait for the next connection
    }

    /* if (...) { close(sockfd); return 1; } */

    printf("Client connected!\n");

    // communtication
    sleep(20);
    write(client_fd, "Hello there, client!\n", 21);

    // close
    printf("Closing client connection...\n");
    close(client_fd);
  }
  printf("Exiting...");

  return EXIT_SUCCESS;
}
