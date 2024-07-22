//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  thread_data_t* thread_data = (thread_data_t*)data;
  simulation_t* simulation = thread_data->simulation;
  thread_data->self = pthread_self();

  while (true) {
    size_t my_unit = 0;
    pthread_mutex_lock(&simulation->can_access_next_unit);
    if (simulation->next_unit < simulation->unit_count) {
      my_unit = ++simulation->next_unit;
    } else {
      pthread_mutex_unlock(&simulation->can_access_next_unit);
      break;
    }
    pthread_mutex_unlock(&simulation->can_access_next_unit);

    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));
    queue_enqueue(&simulation->queue, my_unit);
    printf("Thread %zu Produced %zu\n", thread_data->thread_number, my_unit);
    sem_post(&simulation->can_consume);
  }

  return NULL;
}
