// Copyright 2023 Sebastian Venegas
// Simulates a producer and a consumer that share a bounded buffer

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "simulation.h"

# if 0
typedef struct  {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

#endif

// int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
// int create_threads(shared_data_t* shared_data);
// void* produce(void* data);
// void* consume(void* data);
// useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    error = simulation_run(shared_data, argc, argv);
    simulation_destroy(shared_data);
  } else {
    fprintf(stderr, "error: could not allocate simulation\n");
    error = EXIT_FAILURE;
  }
  return error;
}
