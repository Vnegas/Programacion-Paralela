//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// thread_shared_data_t
typedef struct shared_data {
  size_t life_pinata;
  pthread_mutex_t can_access_pinata;
  size_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  bool broke_pinata;
  size_t thread_number;  // rank
  size_t hit_number;
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Simulates every thread hitting the pinata, every hit decrements the 
 * "life" of the pinata. Function made for threads to work on it.
 * @param private_data[index], private information of every thread to work in 
 * the function. It must be initialized. Private information is number of hits 
 * it has at the moment, its thread number and a pointer to the shared_data.
 */
void* hit_pinata(void* data);
/**
 * @brief Creates and initialize private_data for each thread, and creates every
 * thread.
 * @param shared_data, public information accesible to every thread. It must be 
 * initialized. Public information is total number of threads and the life of 
 * the pinata.
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
  size_t life_pinata = 0;
  if (argc == 3) {
    if (sscanf(argv[1], "%zu", &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
    if (sscanf(argv[2], "%zu", &life_pinata) == 1) {
    } else {
      fprintf(stderr, "Error: invalid number of hits\n");
      return 12;
    }
  } else {
    fprintf(stderr, "Error: invalid number of arguments\n");
    return 13;
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    error = pthread_mutex_init(&shared_data->can_access_pinata, /*attr*/NULL);
    if (error == EXIT_SUCCESS) {
      shared_data->thread_count = thread_count;
      shared_data->life_pinata = life_pinata;

      error = create_threads(shared_data);

      pthread_mutex_destroy(&shared_data->can_access_pinata);
      free(shared_data);
    } else {
      fprintf(stderr, "Error: could not init mutex\n");
      return 14;
    }
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 15;
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
      private_data[thread_number].broke_pinata = false;
      private_data[thread_number].hit_number = 0;
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, hit_pinata
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    /*
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      printf("Thread %zu/%zu: %zu hits\n", private_data[thread_number].thread_number
        , shared_data->thread_count, private_data[thread_number].hit_number);
    }*/

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %zu threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

// procedure race:
void* hit_pinata(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  pthread_mutex_lock(&shared_data->can_access_pinata);
  while (shared_data->life_pinata > 0) {
    // lock(can_access_position)
    // race condition/data race/condición de carrera:
    // modificación concurrente de memoria compartida
  --shared_data->life_pinata;
  ++private_data->hit_number;
  if (shared_data->life_pinata == 0) {
    private_data->broke_pinata = true;
  }
  }
  pthread_mutex_unlock(&shared_data->can_access_pinata);
  
  printf("Thread %zu/%zu: %zu hits\n", private_data->thread_number
    , shared_data->thread_count, private_data->hit_number);
      
  return NULL;
}  // end procedure
