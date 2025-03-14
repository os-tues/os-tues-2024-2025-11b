#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define COUNT_UP_TO 1000000

int main(void) {
  int mem_fd = shm_open("multiprocessing_introduction", O_RDWR | O_CREAT, 0600);
  if (mem_fd == -1) {
    perror("shm_open");
    return 1;
  }

  size_t mem_size = sizeof(unsigned int);

  if (ftruncate(mem_fd, mem_size) == -1) {
    perror("ftruncate");
    return 1;
  }

  unsigned int *count_ptr =
      mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);

  sem_t *sem_fd =
      sem_open("multiprocessing_introduction", O_RDWR | O_CREAT, 0600, 1);
  if (sem_fd == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    // Child
    for (int i = 0; i < COUNT_UP_TO; i++) {
      sem_wait(sem_fd);
      (*count_ptr)++;
      if (i % 10 == 0) {
        printf("Child count: %d\n", *count_ptr);
      }
      sem_post(sem_fd);
    }
    munmap(count_ptr, mem_size);

    // Usage after unmap; broken.
    /* printf("\naaaaaaaaaaaaaaaaaaaaaaa"); */
    /* fflush(stdout); */
    /* printf("Child count (broken): %d\n", *count_ptr); */

    sem_close(sem_fd);
    close(mem_fd);
  }

  if (pid > 0) {
    // Parent
    for (int i = 0; i < COUNT_UP_TO; i++) {
      sem_wait(sem_fd);
      (*count_ptr)++;
      if (i % 10 == 0) {
        /* printf("Parent count: %d\n", *count_ptr); */
      }
      sem_post(sem_fd);
    }

    sem_close(sem_fd);

    // Wait for child to finish
    wait(NULL);

    // Unlink semaphore
    sem_unlink("multiprocessing_introduction");

    // Print the overall count.
    printf("Overall count: %d\n", *count_ptr);

    // Close and delete shared memory
    munmap(count_ptr, mem_size);
    close(mem_fd);
    shm_unlink("multiprocessing_introduction");
  }
  return 0;
}
