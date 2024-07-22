//  Copyright 2023 Sebastian Venegas

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define INITIAL_CAPACITY 100
#define INCREASE_FACTOR  10

int array_init(shared_data_t* shared_data) {
  assert(shared_data);
  int error = EXIT_SUCCESS;
  shared_data->thread_count = 0;
  shared_data->count = 0;
  shared_data->max_char_pass = 0;
  shared_data->alphabet = (char*) calloc(INITIAL_CAPACITY, sizeof(char));
  shared_data->files = (char**) calloc(INITIAL_CAPACITY, sizeof(char));
  if (shared_data->alphabet == NULL) {
    error = EXIT_FAILURE;
  }
  return error;
}

int array_uninit(shared_data_t* shared_data) {
  assert(shared_data);
  for (size_t i = 0; i < shared_data->count; ++i) {
    free(shared_data->files[i]);
    free(shared_data->passwords[i]);
  }
  free(shared_data->pass_found);
  free(shared_data->files);
  free(shared_data->passwords);
  free(shared_data->alphabet);
  shared_data->thread_count = shared_data->max_char_pass = 0;
  shared_data->count = 0;
  shared_data->pass_found = false;
  return EXIT_SUCCESS;
}

int array_append(shared_data_t* shared_data, char* file_path) {
  assert(shared_data);
  int error = EXIT_SUCCESS;

  shared_data->files[shared_data->count] = (char*) calloc(300, sizeof(char));
  if (!shared_data->files[shared_data->count]) {
    error = EXIT_FAILURE;
  } else {
    strcpy(shared_data->files[shared_data->count], file_path);
  }

  if (error == EXIT_SUCCESS) {
    ++shared_data->count;
  }
  return error;
}
