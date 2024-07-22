//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include "producer.h"

void* produce(void* data) {
  simulation_t* simulation = (simulation_t*)data;
  size_t characters_count = strlen(simulation->alphabet);
  for (size_t i = 1; i < simulation->max_char_pass+1; ++i) {
    size_t workspace = pow(characters_count, i);

    char* pass = (char*) calloc (simulation->max_char_pass * 2, sizeof(char));
    size_t* index_pass = (size_t*) calloc
      (simulation->max_char_pass * 2, sizeof(size_t));

    for (size_t j = 0; j < workspace; ++j) {
      bool keep_going = false;
      // tobase and recieve it
      size_t index = 0;  // Initialize index of result
      // Convert input number is given base by repeatedly
      // dividing it by base and taking remainder
      size_t number = j; size_t base = characters_count;
      while (number > 0) {
        index_pass[index] = (number % base);
        number /= base;
        ++index;
      }
      size_t size_index = index;
      if (index == 0) {
        size_index = 1;
      }
      for (size_t k = 0; k < size_index; ++k) {
        pass[k] = simulation->alphabet[index_pass[k]];
      }
      // produce
      queue_enqueue(&simulation->queue, pass, simulation->max_char_pass*2);
      // signals that queue has element to consume
      sem_post(&simulation->can_consume);

      if (index == 0) {
        index_pass[index] = 0;
      } else {
        index_pass[index-1] = 0;
      }
      for (size_t k = 0; k < size_index; ++k) {
        pass[k] = simulation->alphabet[index_pass[k]];
      }
      // produce
      queue_enqueue(&simulation->queue, pass, simulation->max_char_pass*2);
      // signals that queue has element to consume
      sem_post(&simulation->can_consume);

      pthread_mutex_lock(&simulation->can_access_pass_found);
      for (size_t cont = 0; cont < simulation->file_count; ++cont) {
        if (!simulation->pass_found[cont]) {
          keep_going = true;
        }
      }
      pthread_mutex_unlock(&simulation->can_access_pass_found);

      if (!keep_going) {
        for (size_t cont = 0; cont < simulation->consumer_count; ++cont) {
          queue_enqueue(&simulation->queue, "", simulation->max_char_pass);
          sem_post(&simulation->can_consume);
        }
        free(pass);
        free(index_pass);
        return NULL;
      }
    }
    free(pass);
    free(index_pass);
  }
  for (size_t i = 0; i < simulation->consumer_count; ++i) {
    queue_enqueue(&simulation->queue, "", simulation->max_char_pass);
    sem_post(&simulation->can_consume);
  }
  return NULL;
}
