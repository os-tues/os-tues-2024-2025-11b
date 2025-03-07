#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *a_ptr;
  char *str;
} thread_function_arg;

void *thread_function(void *arg);

int main(void) {
  pthread_t thread[5];
  int a = 5;
  char str[10] = "test";

  thread_function_arg arg;
  arg.a_ptr = &a;
  arg.str = str;

  for (int i = 0; i < 5; i++) {

    int ret = pthread_create(&thread[i], NULL, &thread_function, &arg);
    if (ret != 0) {
      printf("error\n");
      // handle error
    }
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(thread[i], NULL);
  }

  printf("Main thread finished.\n");

  /* printf("%lb", thread); */
  return EXIT_SUCCESS;
}

void *thread_function(void *arg) {
  thread_function_arg *arg_parsed = arg;
  for (int i = 0; i < 5; i++) {
    /* printf("%d\n", *arg_parsed->a_ptr); */
    /* printf("%s\n", arg_parsed->str); */
  }

  printf("Thread function finished.\n");

  return NULL;
}
