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
  /* pthread_attr_setdetachstate( */
  /*     &thread_attributes, */
  /*     PTHREAD_CREATE_DETACHED); // TODO: check why not running */

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
  }

  for (int i = 0; i < 5; i++) {
    if (pthread_join(thread[i], NULL) != 0) {
      // handle error
    }
  }

  printf("Main thread finished.\n");
  return EXIT_SUCCESS;
}

void *thread_function(void *arg) {
  thread_function_arg *arg_parsed = arg;
  /* printf("%d\n", *arg_parsed->a_ptr); */
  /* printf("%s\n", arg_parsed->str); */

  pthread_t thread;
  int ret = pthread_create(&thread, NULL, &thread_function, &arg);
  if (ret != 0) {
    if (ret == EAGAIN) {
      printf("Insufficient resources.\n");
    }
  }

  pthread_join(thread, NULL);

  printf("Thread function finished.\n");
  return NULL;
}
