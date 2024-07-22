// Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

/** 
 * @brief shared data between threads
 */
typedef struct shared_data {
  size_t philosopher_num;
  useconds_t min_think;
  useconds_t max_think;
  useconds_t min_eat;
  useconds_t max_eat;
  sem_t* chopsticks;  //  array of semaphores
  pthread_mutex_t can_print;  //  mutex from pthread.h
} shared_data_t;

typedef struct private_data {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Create philosopher_num threads
 * @param shared_data
 * @return error code
 */
int create_threads(shared_data_t* shared_data);
/**
 * @brief Process input from stdin
 * @param argc
 * @param argv
 * @param shared_data
 * @return error code
 */
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
/**
 * @brief Simulates dining_philosophers
 * @param data
 */
void* philosopher(void* data);
/**
 * @brief simulates philosopher thinking
 * @param data
 */
void think(private_data_t* data);
/**
 * @brief simulates philosopher eating
 * @param data
 */
void eat(private_data_t* data);
/**
 * @brief Get the left chopstick
 * @param data
 */
void get_left_chopstick(private_data_t* data);
/**
 * @brief Get the right chopstick
 * @param data
 */
void get_right_chopstick(private_data_t* data);
/**
 * @brief Leaves/puts/liberates the left chopstick
 * @param data
 */
void put_left_chopstick(private_data_t* data);
/**
 * @brief Leaves/puts/liberates the right chopstick
 * @param data
 */
void put_right_chopstick(private_data_t* data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      // allocate memory for the array of semaphores
      shared_data->chopsticks = (sem_t*) calloc(shared_data->philosopher_num
        , sizeof(sem_t));
      // initialize mutex        
      error += pthread_mutex_init(&shared_data->can_print, /*attr*/ NULL);
      // initialize the semaphores one by one
      if (error == EXIT_SUCCESS && shared_data->chopsticks) {
        for (size_t index = 0; index < shared_data->philosopher_num; ++index) {
          sem_init(&shared_data->chopsticks[index], /*pshared*/ 0, /*value*/ 1);
        }

        error = create_threads(shared_data);

        // we destroy everything in inverse order in which we created it them
        for (size_t index = 0; index < shared_data->philosopher_num; ++index) {
          sem_destroy(&shared_data->chopsticks[index]);
        }
        // destroy the mutex
        pthread_mutex_destroy(&shared_data->can_print);
        // free the allocated memory for array of semaphors
        free(shared_data->chopsticks);
      } else {
        fprintf(stderr, "error: could not init memory\n");
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

int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  if (argc == 6) {
    if (sscanf(argv[1], "%zu", &shared_data->philosopher_num) != 1
      || shared_data->philosopher_num == 0) {
      fprintf(stderr, "invalid philosopher count: %s\n", argv[1]);
      return 11;
    }

    if (sscanf(argv[2], "%u", &shared_data->min_think) != 1) {
      fprintf(stderr, "invalid min think duration: %s\n", argv[2]);
      return 12;
    }

    if (sscanf(argv[3], "%u", &shared_data->max_think) != 1) {
      fprintf(stderr, "invalid max think duration: %s\n", argv[3]);
      return 13;
    }

    if (sscanf(argv[4], "%u", &shared_data->min_eat) != 1) {
      fprintf(stderr, "invalid min eat duration: %s\n", argv[4]);
      return 14;
    }

    if (sscanf(argv[5], "%u", &shared_data->max_eat) != 1) {
      fprintf(stderr, "invalid max eat duration: %s\n", argv[5]);
      return 15;
    }
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "usage: philosophers, min think duration, max think duration, min eat duration, max eat duration\n");
    return 10;
  }
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  const size_t thread_count = shared_data->philosopher_num;
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (size_t index = 0; index < thread_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, philosopher
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
      , shared_data->philosopher_num);
    error = 22;
  }
  return error;
}

void* philosopher(void* data) {
  private_data_t* private_data = (private_data_t*)data;

  const size_t rank = private_data->thread_number;
  while (true) {
    think(private_data);
    if ((rank % 2) == 0) {
      get_right_chopstick(private_data);
      get_left_chopstick(private_data);
    } else {
      get_left_chopstick(private_data);
      get_right_chopstick(private_data);
    }
    eat(private_data);
    put_left_chopstick(private_data);
    put_right_chopstick(private_data);
  }

  return NULL;
}

void think(private_data_t* data) {
  shared_data_t* shared = data->shared_data;
  pthread_mutex_lock(&shared->can_print);
    printf("Philosopher %zu is thinking\n", data->thread_number);
  pthread_mutex_unlock(&shared->can_print);
  unsigned int seed = pthread_self() ^ time(NULL) ^ getpid();
  long number = (long)(rand_r(&seed) % shared->max_think) + shared->min_think;
  usleep(1000 * number);
}

void eat(private_data_t* data) {
  shared_data_t* shared = data->shared_data;
  pthread_mutex_lock(&shared->can_print);
    printf("Philosopher %zu is eating\n", data->thread_number);
  pthread_mutex_unlock(&shared->can_print);
  unsigned int seed = pthread_self() ^ time(NULL) ^ getpid();
  long number = (long)(rand_r(&seed) % shared->max_eat) + shared->min_eat;
  usleep(1000 * number);
}

void get_left_chopstick(private_data_t* data) {
  size_t left = (data->thread_number + 1) % data->shared_data->philosopher_num;
  sem_wait(&data->shared_data->chopsticks[left]);
}

void get_right_chopstick(private_data_t* data) {
  sem_wait(&data->shared_data->chopsticks[data->thread_number]);
}

void put_left_chopstick(private_data_t* data) {
  size_t left = (data->thread_number + 1) % data->shared_data->philosopher_num;
  sem_post(&data->shared_data->chopsticks[left]);
}

void put_right_chopstick(private_data_t* data) {
  sem_post(&data->shared_data->chopsticks[data->thread_number]);
}
