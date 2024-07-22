//  Copyright 2023 Sebastian Venegas

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define INITIAL_CAPACITY 100
#define INCREASE_FACTOR  10

/**
 * @brief if necessary, can increase the size of the array and the capacity of
 * element it can contain
 * @param array 
 * @return int error code
 */
int increase_capacity(struct array* array);

int array_init(struct array* array) {
  assert(array);
  int error = EXIT_SUCCESS;
  array->count = 0;
  array->max_char_pass = 0;
  array->capacity = INITIAL_CAPACITY;
  array->alphabet = (char*) calloc(array->capacity, sizeof(char));
  array->elements = (data_t*) calloc(array->capacity, sizeof(data_t));
  if (array->elements == NULL || array->alphabet == NULL) {
    array->capacity = 0;
    error = EXIT_FAILURE;
  }
  return error;
}

int array_uninit(struct array* array) {
  assert(array);
  for (size_t i = 0; i < array->count; ++i) {
    free(array->elements[i].file);
    free(array->elements[i].password);
    free(array->elements[i].index_pass);
  }
  free(array->elements);
  free(array->alphabet);
  array->max_char_pass = array->count = array->capacity = 0;
  return EXIT_SUCCESS;
}

int array_append(struct array* array, char* file_path) {
  assert(array);
  int error = EXIT_SUCCESS;
  if (array->count == array->capacity) {
    error = increase_capacity(array);
  }
  if (error == EXIT_SUCCESS) {
    array->elements[array->count].file = (char*) calloc(300, sizeof(char));
    if (!array->elements->file) {
      error = EXIT_FAILURE;
      array->count--;
    } else {
      strcpy(array->elements[array->count].file, file_path);
      array->elements[array->count].password = (char*) calloc(array->
        max_char_pass * 2, sizeof(char));
        array->elements[array->count].index_pass = (size_t*) calloc(array->
        max_char_pass * 2, sizeof(size_t));
      if ((!array->elements->password) || (!array->elements->index_pass)) {
        error = EXIT_FAILURE;
      }
    }
  }
  if (error == EXIT_SUCCESS) {
    ++array->count;
  }
  return error;
}

int increase_capacity(struct array* array) {
  assert(array);
  int error = EXIT_SUCCESS;
  size_t new_capacity = INCREASE_FACTOR * array->capacity;
  data_t* new_elements = (data_t*) calloc(new_capacity, sizeof(data_t));
  if (new_elements) {
    for (size_t index = 0; index < array->count; ++index) {
      strcpy(new_elements[index].file, array->elements[index].file);
      strcpy(new_elements[index].password, array->elements[index].password);
    }
    free(array->elements->index_pass);
    free(array->elements);
    array->elements = new_elements;
    array->capacity = new_capacity;
  } else {
    error = EXIT_FAILURE;
  }
  return error;
}
