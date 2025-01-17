#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#define BYTES_TO_READ 10

int main(int argc, char **argv) {
  // Check if correct amount of args is supplied.
  if (argc != 2) {
    fprintf(stderr, "Invalid arguments.");
    return 1;
  }

  // Open the file specified in argv.
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // Variant 1 for reading from file.
  /* char read_buf[10]; */
  /* int bytes_read = read(fd, read_buf, 10); */
  /* while (bytes_read != 0) { */
  /*   bytes_read = read(fd, read_buf, 10); */
  /*   write(STDIN_FILENO, read_buf, 10); */
  /* } */

  // Variant 2 for reading from file.
  /* char read_buf[10]; */
  /* int bytes_read; */
  /* while ((bytes_read = read(fd, read_buf, 10)) != 0) { */
  /*   write(STDIN_FILENO, read_buf, 10); */
  /* } */

  // Variant 3 for reading from file.
  char read_buf[BYTES_TO_READ];
  int bytes_read;
  do {
    // Read from file.
    bytes_read = read(fd, read_buf, BYTES_TO_READ);
    if (bytes_read == -1) {
      perror("read");
      close(fd);
      return -1;
    }

    // Print the number of bytes read.
    /* printf("<[%d]>\n", bytes_read); */

    // Write to the standard output
    if (write(STDIN_FILENO, read_buf, bytes_read) == -1) {
      perror("write");
    }
  } while (bytes_read == BYTES_TO_READ);
  /* } while (bytes_read != 0); */

  // Close file
  if (close(fd) == -1) {
    perror("open");
    return 1;
  }

  return 0;
}
