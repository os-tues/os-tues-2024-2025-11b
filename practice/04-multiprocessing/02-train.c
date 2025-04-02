#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void send_passangers_to_next(unsigned int passanger_count,
                             const char *next_station_fifo_name);

void receive_passangers_loop(const char *previous_station_fifo_name,
                             const char *previous_station_name, int has_next,
                             const char *next_station_fifo_name,
                             const char *next_station_name);

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Invalid argument count");
    return 1;
  }

  int has_previous = strcmp("null", argv[2]);
  int has_next = strcmp("null", argv[3]);

  char *current_station_name = argv[1];
  char *previous_station_name = argv[2];
  char *next_station_name = argv[3];

  printf("Starting metro station \"%s\"...\n", current_station_name);
  printf("Previous station: %s\n",
         has_previous ? previous_station_name : "null");
  printf("Next station: %s\n", has_next ? next_station_name : "null");

  char previous_station_fifo_name[100] = "train_from_";
  strcat(previous_station_fifo_name, previous_station_name);
  strcat(previous_station_fifo_name, "_to_");
  strcat(previous_station_fifo_name, current_station_name);

  char next_station_fifo_name[100] = "train_from_";
  strcat(next_station_fifo_name, current_station_name);
  strcat(next_station_fifo_name, "_to_");
  strcat(next_station_fifo_name, next_station_name);

  pthread_t receive_passangers_thread;
  if (has_previous) {
    if (mkfifo(previous_station_fifo_name, 0700) == -1 && errno != EEXIST) {
      perror("previous station fifo creation");
      return 1;
    }

    pthread_create(&receive_passangers_thread, NULL,
                   &receive_passangers_loop, );
  }

  if (has_next) {
    if (mkfifo(next_station_fifo_name, 0700) == -1 && errno != EEXIST) {
      perror("next station fifo creation");
      return 1;
    }

    while (1) {
      int passanger_count;
      printf("Input passanger_count: ");
      if (scanf("%d", &passanger_count) == EOF) {
        break;
      }

      // TODO: Input validation;

      send_passangers_to_next(passanger_count, next_station_fifo_name);

      printf("Sending %d passangers to %s...\n", passanger_count,
             next_station_name);
    }
  }

  return 0;
}

void send_passangers_to_next(unsigned int passanger_count,
                             const char *next_station_fifo_name) {
  int next_station_fd = open(next_station_fifo_name, O_WRONLY);
  write(next_station_fd, &passanger_count, sizeof(passanger_count));
  close(next_station_fd);
}

void receive_passangers_loop(const char *previous_station_fifo_name,
                             const char *previous_station_name, int has_next,
                             const char *next_station_fifo_name,
                             const char *next_station_name) {
  int previous_station_fd = open(previous_station_fifo_name, O_RDONLY);
  while (1) {
    int passanger_count;
    read(previous_station_fd, &passanger_count, sizeof(passanger_count));
    printf("From %s, %d passangers have arrived...\n", previous_station_name,
           passanger_count);

    if (has_next) {
      send_passangers_to_next(passanger_count, next_station_fifo_name);
    }
  }
  close(previous_station_fd);
}
