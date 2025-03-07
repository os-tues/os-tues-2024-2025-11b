#include <errno.h>
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

  pthread_attr_t thread_attributes;
  pthread_attr_init(&thread_attributes);
  // Way 1 of configuring a thread as detached
  pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED);

  thread_function_arg arg;
  arg.a_ptr = &a;
  arg.str = str;

  for (int i = 0; i < 5; i++) {
    int ret =
        pthread_create(&thread[i], &thread_attributes, &thread_function, &arg);

    if (ret != 0) {
      printf("error\n");
      // handle error
    }

    // Way 2 of configuring a thread as detached
    if (pthread_detach(thread[i]) != 0) {
      // handle error
      printf("detach error\n");
    }
  }

  malloc(8);

  printf("Main thread finished.\n");
  return EXIT_SUCCESS;
}

void *thread_function(void *arg) {
  /* thread_function_arg *arg_parsed = arg; */
  /* printf("%d\n", *arg_parsed->a_ptr); */
  /* printf("%s\n", arg_parsed->str); */

  printf("Thread function finished.\n");
  return NULL;
}
