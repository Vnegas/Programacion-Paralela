//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  simulation_t* simulation = (simulation_t*)data;

  while (true) {
    size_t my_unit = 0;
    // bool error = ;
    sem_wait(&simulation->can_produce);
    pthread_mutex_lock(&simulation->can_access_next_unit);
    if (simulation->next_unit < simulation->unit_count) {
      pthread_mutex_unlock(&simulation->can_access_next_unit);
      break;
    }
    my_unit = ++simulation->next_unit;
    pthread_mutex_unlock(&simulation->can_access_next_unit);

    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));
    
    queue_enqueue(&simulation->queue, my_unit);
    printf("Produced %zu\n", my_unit);
    sem_post(&simulation->can_consume);
  }

  return NULL;
}
