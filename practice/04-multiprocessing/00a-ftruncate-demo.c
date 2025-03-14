#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  int fd = open("example.txt", O_RDWR | O_CREAT, 0600);
  if (fd == -1) {
    perror("open");
  }
  if (ftruncate(fd, 10) == -1) {
    perror("ftruncate");
  }
  return 0;
}
