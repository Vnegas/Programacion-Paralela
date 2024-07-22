//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <assert.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t
typedef struct shared_data {
  sem_t* can_greet;
  size_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Prints a greet of every thread in order, it uses semaphores as control
 * of concurrency.
 * @param private_data[index], private information of every thread to work in 
 * the function. It must be initialized. Private information is the thread
 * number or rank and a pointer to the shared_data.
 */
void* greet(void* data);
/**
 * @brief Creates and initialize private_data for each thread, and creates every
 * thread.
 * @param shared_data, public information accesible to every thread. It must be 
 * initialized. Public information is total number of threads and an array of 
 * semaphores.
 * @return An error code:
 *  0  for success.
 *  21 if could not create secondary thread
 *  22 if could not allocate n number of threads
*/
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->can_greet = (sem_t*) calloc(thread_count, sizeof(sem_t));
    shared_data->thread_count = thread_count;

    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      // can_greet[thread_number] := create_semaphore(not thread_number)
      shared_data->can_greet[thread_number] = *sem_open("/can_greet", /*flag*/ 
        O_CREAT, /*mode*/ S_IRUSR | S_IWUSR, /*value*/ !thread_number);
    }

    if (shared_data->can_greet) {
      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

      printf("Execution time: %.9lfs\n", elapsed_time);

      free(shared_data->can_greet);
    } else {
      fprintf(stderr, "Error: could not allocate semaphores\n");
      error = 13;
    }

    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      error = sem_unlink("/can_greet");
    }
    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    error = 12;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
      sem_close(&shared_data->can_greet[thread_number]);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %zu threads\n"
      , shared_data->thread_count);
    error = 23;
  }

  return error;
}

// procedure greet:
void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // Wait until it is my turn
  // wait(can_greet[thread_number])
  int error = sem_wait(&shared_data->can_greet[private_data->thread_number]);
  if (error) {
    fprintf(stderr, "error: could not wait for semaphore\n");
  }

  // print "Hello from secondary thread"
  printf("Hello from secondary thread %zu of %zu\n"
    , private_data->thread_number+1, shared_data->thread_count);

  // Allow subsequent thread to do the task
  // signal(can_greet[(thread_number + 1) mod thread_count])
  const size_t next_thread = (private_data->thread_number + 1)
    % shared_data->thread_count;

  error = sem_post(&shared_data->can_greet[next_thread]);
  if (error) {
    fprintf(stderr, "error: could not increment semaphore\n");
  }

  return NULL;
}  // end procedure