#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10

typedef struct {
  int *buffer;
  int *count;
  int value_to_store;
  pthread_mutex_t *buffer_mutex;
  pthread_cond_t *cond_not_empty;
  pthread_cond_t *cond_not_full;
} producer_function_args_t;

typedef struct {
  int *buffer;
  int *count;
  pthread_mutex_t *buffer_mutex;
  pthread_cond_t *cond_not_empty;
  pthread_cond_t *cond_not_full;
} consumer_function_args_t;

void *producer_function(void *arg);

void *consumer_function(void *arg);

void print_buffer(int *buffer);

int main(void) {
  const int CONSUMERS_COUNT = 2;
  const int PRODUCERS_COUNT = 10;
  int buffer[BUFFER_SIZE];
  int count = 0; // number of items in buffer

  // Initialize the buffer with 0s
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }

  // Initialize the buffer mutex
  pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
  /* pthread_mutex_init(&buffer_mutex, NULL); */

  // Initialize thread conditions
  pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;
  pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;

  // Create the producer and consumer threads
  producer_function_args_t producer_function_args;
  producer_function_args.buffer = buffer;
  producer_function_args.count = &count;
  producer_function_args.buffer_mutex = &buffer_mutex;
  producer_function_args.cond_not_empty = &cond_not_empty;
  producer_function_args.cond_not_full = &cond_not_full;

  pthread_t producer_threads[PRODUCERS_COUNT];
  for (int i = 0; i < PRODUCERS_COUNT; i++) {
    producer_function_args.value_to_store = i;
    pthread_create(&producer_threads[i], NULL, producer_function,
                   (void *)&producer_function_args);
  }

  consumer_function_args_t consumer_function_args;
  consumer_function_args.buffer = buffer;
  consumer_function_args.count = &count;
  consumer_function_args.buffer_mutex = &buffer_mutex;
  consumer_function_args.cond_not_empty = &cond_not_empty;
  consumer_function_args.cond_not_full = &cond_not_full;

  pthread_t consumer_threads[CONSUMERS_COUNT];
  for (int i = 0; i < CONSUMERS_COUNT; i++) {
    pthread_create(&consumer_threads[i], NULL, consumer_function,
                   (void *)&consumer_function_args);
  }

  // Join threads
  for (int i = 0; i < PRODUCERS_COUNT; i++) {
    pthread_join(producer_threads[i], NULL);
  }

  // Destroy resources
  pthread_mutex_destroy(&buffer_mutex);
  pthread_cond_destroy(&cond_not_empty);

  // Print the state of the buffer
  print_buffer(buffer);

  return EXIT_SUCCESS;
}

void *producer_function(void *arg) {
  producer_function_args_t parsed_args = *((producer_function_args_t *)arg);

  for (int i = 0; i < 2; i++) {
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(parsed_args.buffer_mutex);

    while (*(parsed_args.count) == BUFFER_SIZE) {
      pthread_cond_wait(parsed_args.cond_not_full, parsed_args.buffer_mutex);
    }

    parsed_args.buffer[*parsed_args.count] = parsed_args.value_to_store;
    *(parsed_args.count) += 1;

    printf("Produced: %d; ", parsed_args.value_to_store);
    print_buffer(parsed_args.buffer);

    pthread_cond_signal(parsed_args.cond_not_empty);
    pthread_mutex_unlock(parsed_args.buffer_mutex);
  }

  return NULL;
}

void *consumer_function(void *arg) {
  consumer_function_args_t parsed_args = *((consumer_function_args_t *)arg);

  for (int i = 0; i < 2; i++) {
    // Simulate work for 1-3 seconds
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(parsed_args.buffer_mutex);

    /* if (*(parsed_args.count) == 0) { */
    /*   printf("Buffer is empty\n"); */
    /*   pthread_mutex_unlock(parsed_args.buffer_mutex); */
    /*   continue; */
    /* } */

    while (*(parsed_args.count) == 0) {
      pthread_cond_wait(parsed_args.cond_not_empty, parsed_args.buffer_mutex);
    }

    int consumed = parsed_args.buffer[*(parsed_args.count) - 1];
    parsed_args.buffer[*(parsed_args.count) - 1] = 0;
    *(parsed_args.count) -= 1;

    printf("Consumed: %d; ", consumed);
    print_buffer(parsed_args.buffer);

    pthread_cond_signal(parsed_args.cond_not_full);
    pthread_mutex_unlock(parsed_args.buffer_mutex);
  }

  return NULL;
}

void print_buffer(int *buffer) {
  printf("Buffer: ");
  for (int i = 0; i < BUFFER_SIZE; i++) {
    printf("%d ", buffer[i]);
  }
  printf("\n");
}
