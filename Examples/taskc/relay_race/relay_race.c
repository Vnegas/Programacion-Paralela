// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** 
 * @brief shared data between threads
 */
typedef struct shared_data {
  size_t team_count;
  useconds_t stage1_duration;
  useconds_t stage2_duration;
  size_t position;
  pthread_barrier_t start_barrier;  //  a barrier from pthread.h
  sem_t* batons;  //  array of semaphores
  pthread_mutex_t finish_mutex;  //  mutex from pthread.h
} shared_data_t;

typedef struct private_data {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

int create_threads(shared_data_t* shared_data);
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
void* start_race(void* data);
void* finish_race(void* data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->position = 0;
      // initialize the barrier
      // a restricted pointers is when a pointer must be accessed through a
      // pointer an only one pointer
      error = pthread_barrier_init(&shared_data->start_barrier,
        /*attr*/ NULL, /*count*/ shared_data->team_count);
      shared_data->batons = (sem_t*) calloc(shared_data->team_count
        , sizeof(sem_t));  //  allocate memory for the array of semaphores
      //  initialize mutex        
      error += pthread_mutex_init(&shared_data->finish_mutex, /*attr*/ NULL);
      // initialize the semaphores one by one
      if (error == EXIT_SUCCESS && shared_data->batons) {
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_init(&shared_data->batons[index], /*pshared*/ 0, /*value*/ 0);
        }

        struct timespec start_time, finish_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        clock_gettime(CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed_time);
        // we destroy everything in inverse order in which we created it them
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_destroy(&shared_data->batons[index]);
        }
        // destroy the mutex
        pthread_mutex_destroy(&shared_data->finish_mutex);
        // destroy the barrier
        free(shared_data->batons);
        // destroy the barrier
        pthread_barrier_destroy(&shared_data->start_barrier);
      } else {
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}
/**
 * @brief analyze the arguments
 * @param argc number of arguments
 * @param argv array of arguments
 * @return error code
 */
int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  if (argc == 4) {
    if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
      || shared_data->team_count == 0 ) {
      fprintf(stderr, "invalid team count: %s\n", argv[1]);
      return 11;
    }

    if ( sscanf(argv[2], "%u", &shared_data->stage1_duration) != 1 ) {
      fprintf(stderr, "invalid stage 1 duration: %s\n", argv[2]);
      return 12;
    }

    if ( sscanf(argv[3], "%u", &shared_data->stage2_duration) != 1 ) {
      fprintf(stderr, "invalid stage 2 duration: %s\n", argv[3]);
      return 13;
    }
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "usage: relay_race teams stage1duration stage2duration\n");
    return 10;
  }
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;

  const size_t thread_count = 2 * shared_data->team_count;
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (size_t index = 0; index < shared_data->team_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = shared_data->team_count; index < thread_count;
        ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->team_count);
    error = 22;
  }

  return error;
}
/**
 * @brief start the race
 * 
 * @param data private data of the thread
 */
void* start_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  const size_t rank = private_data->thread_number;
  const size_t team_number = rank;
  // wait for all threads to be created
  pthread_barrier_wait(&shared_data->start_barrier);
  usleep(1000 * shared_data->stage1_duration);
  // signal the next thread in the team that it can start.
  // by signaling the team semaphore.
  sem_post(&shared_data->batons[team_number]);

  return NULL;
}
/**
 * @brief finish the race
 * 
 * @param data private data of a thread
 */
void* finish_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  const size_t rank = private_data->thread_number;
  const size_t team_number = rank - shared_data->team_count;
  assert(team_number < shared_data->team_count);

  // wait for the previous thread in the team to signal that it has finished.
  sem_wait(&shared_data->batons[team_number]);
  usleep(1000 * shared_data->stage2_duration);
  // it represents the finish line
  pthread_mutex_lock(&shared_data->finish_mutex);
  const size_t our_position = ++shared_data->position;
    printf("Place %zu: team %zu\n", our_position, team_number);
  pthread_mutex_unlock(&shared_data->finish_mutex);

  return NULL;
}
