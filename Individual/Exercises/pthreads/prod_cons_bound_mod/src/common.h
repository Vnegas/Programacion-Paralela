//  Copyright 2023 Sebastian Venegas

#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

/**
 * @brief struct that acts like a buffer
 */
typedef struct {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  sem_t can_produce;
  sem_t can_consume;
} shared_data_t;

useconds_t random_between(useconds_t min, useconds_t max);

#endif  // COMMON_H
