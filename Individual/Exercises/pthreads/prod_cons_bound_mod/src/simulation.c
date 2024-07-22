//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

#include "consume.h"
#include "common.h"
#include "simulation.h"
#include "produce.h"

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  if (argc == 7) {
    if (sscanf(argv[1], "%zu", &shared_data->buffer_capacity) != 1
      || shared_data->buffer_capacity == 0) {
        fprintf(stderr, "error: invalid buffer capacity\n");
        error = ERR_BUFFER_CAPACITY;
    } else if (sscanf(argv[2], "%zu", &shared_data->rounds) != 1
      || shared_data->rounds == 0) {
        fprintf(stderr, "error: invalid round count\n");
        error = ERR_ROUND_COUNT;
    } else if (sscanf(argv[3], "%u", &shared_data->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[4], "%u", &shared_data->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &shared_data->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[6], "%u", &shared_data->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
      " producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}


int simulation_run(shared_data_t* shared_data, int argc, char* argv[]) {
  int error = analyze_arguments(argc, argv, shared_data);
  if (error == EXIT_SUCCESS) {
    shared_data->buffer = (double*)
        calloc(shared_data->buffer_capacity, sizeof(double));
      if (shared_data->buffer) {
        sem_init(&shared_data->can_produce, 0, shared_data->buffer_capacity);
        sem_init(&shared_data->can_consume, 0, 0);

        unsigned int seed = 0u;
        getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
        srandom(seed);

        struct timespec start_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        struct timespec finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed);

        sem_destroy(&shared_data->can_consume);
        sem_destroy(&shared_data->can_produce);
        free(shared_data->buffer);
      } else {
        fprintf(stderr, "error: could not create buffer\n");
        error = ERR_NOMEM_BUFFER;
      }
    }
  return error;
}

int create_threads(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  pthread_t producer, consumer;
  error = pthread_create(&producer, /*attr*/ NULL, produce, shared_data);
  if (error == EXIT_SUCCESS) {
    error = pthread_create(&consumer, /*attr*/ NULL, consume, shared_data);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "error: could not create consumer\n");
      error = ERR_CREATE_THREAD;
    }
  } else {
    fprintf(stderr, "error: could not create producer\n");
    error = ERR_CREATE_THREAD;
  }

  if (error == EXIT_SUCCESS) {
    pthread_join(producer, /*value_ptr*/ NULL);
    pthread_join(consumer, /*value_ptr*/ NULL);
  }

  return error;
}

void simulation_destroy(shared_data_t* shared_data) {
  assert(shared_data);
  free(shared_data);
}
