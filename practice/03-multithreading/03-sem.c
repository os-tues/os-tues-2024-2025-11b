#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_function(void *arg);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
  int a = 0;

  time_t time_start = time(NULL);

  /* pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; */
  /* pthread_mutex_t mutex; */
  /* pthread_mutex_init(&mutex, NULL); */

  pthread_t thread;
  int ret = pthread_create(&thread, NULL, &thread_function, &a);
  thread_function(&a);
  pthread_join(thread, NULL);
  printf("%d\n", a);

  time_t time_end = time(NULL);
  printf("Total time in seconds: %ld", time_end - time_start);

  return EXIT_SUCCESS;
}

void *thread_function(void *arg) {
  int *a_ptr = (int *)arg;

  for (unsigned int i = 0; i < 10000000; i++) {
    pthread_mutex_lock(&mutex);
    (*a_ptr)++;
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}
