#include <stdio.h>
#include <unistd.h>

void winner();

int main(void) {
  char buf[5];
  gets(buf);
  int cond = 0;
  if (cond) {
    winner();
  }
  return 0;
}

void winner() {
  printf("Congratulations");
  execlp("/bin/cat", "/bin/cat", "/opt/flag.txt");
}
