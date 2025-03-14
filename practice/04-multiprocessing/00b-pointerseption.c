#include <stdio.h>
int main(void) {
  long long int value = 0;
  long long int *a = &value;
  printf("%p\n", a);
  a = (long long int *)&a;
  printf("%p\n", &a);
  printf("%p\n", a);
  printf("%p\n", *a);
  return 0;
}
