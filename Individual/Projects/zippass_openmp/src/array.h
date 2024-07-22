//  Copyright 2023 Sebastian Venegas

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>

/**
 * @brief struct that acts like a dynamic array, contains the number of max
 * character to use in the password, the path the zip file and the password of
 * that zip file
 */
struct data {
  char* file;
  char* password;
  size_t* index_pass;
  size_t size_index_pass;
};
typedef struct data data_t;

/**
 * @brief struct that acts like a dynamic array, contains the count of elements,
 * the capacity it can hold and a pointer to data
 */
struct array {
  size_t thread_count;
  size_t count;
  size_t capacity;
  size_t max_char_pass;
  char* alphabet;
  char** passwords;  // conditionally safe, where to print
  bool* pass_found;
  data_t* elements;
};
typedef struct array array_t;

/**
 * @brief initialize the array and its attributes
 * @param array 
 * @return int error code
 */
int array_init(struct array* array);
/**
 * @brief make free to the array and its elements
 * @param array 
 * @return int error code
 */
int array_uninit(struct array* array);
/**
 * @brief adds new element to the array
 * @param array
 * @param file_path
 * @return int error code
 */
int array_append(struct array* array, char* file_path);

#endif  // ARRAY_H
