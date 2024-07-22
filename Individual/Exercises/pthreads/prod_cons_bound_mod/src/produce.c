//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

#include "produce.h"
#include "common.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  size_t count = 0;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      sem_wait(&shared_data->can_produce);
      usleep(1000 * random_between(shared_data->producer_min_delay
        , shared_data->producer_max_delay));
      shared_data->buffer[index] = ++count;
      printf("Produced %lg\n", shared_data->buffer[index]);
      sem_post(&shared_data->can_consume);
    }
  }

  return NULL;
}
