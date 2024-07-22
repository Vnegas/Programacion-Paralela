//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"

void* consume(void* data) {
  thread_data_t* thread_data = (thread_data_t*)data;
  simulation_t* simulation = thread_data->simulation;
  thread_data->self = pthread_self();
  // thread_data->thread_number += simulation->producer_count;

  while (true) {
    pthread_mutex_lock(&simulation->can_access_consumed_count);
    if (simulation->consumed_count < simulation->unit_count) {
      ++simulation->consumed_count;
    } else {
      pthread_mutex_unlock(&simulation->can_access_consumed_count);
      break;
    }
    pthread_mutex_unlock(&simulation->can_access_consumed_count);
    sem_wait(&simulation->can_consume);
    size_t value = 0;
    
    queue_dequeue(&simulation->queue, &value);
    printf("\t\tConsuming %zu\n", value);
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }
  return NULL;
}
