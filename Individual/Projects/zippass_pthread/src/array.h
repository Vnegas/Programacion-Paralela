//  Copyright 2023 Sebastian Venegas

#ifndef ARRAY_H
#define ARRAY_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

/**
 * @brief struct that acts like a dynamic array, contains the count of elements,
 * the capacity it can hold and a pointer to data
 */
typedef struct shared_data {
  size_t thread_count;
  size_t count;
  size_t max_char_pass;
  char* alphabet;
  char** files;
  bool* pass_found;
  pthread_mutex_t can_access;
  char** passwords;  // conditionally safe, where to print
} shared_data_t;

/**
 * @brief struct that acts like a dynamic array, contains the number of max
 * character to use in the password, the path the zip file and the password of
 * that zip file
 */
typedef struct private_data {
  size_t thread_id;
  char* file;
  size_t file_num;
  size_t char_num;
  char* password;
  size_t* index_pass;
  size_t size_index_pass;
  size_t start_work;
  size_t finish_work;
  shared_data_t* shared_data;
} private_data_t;
/**
 * @brief initialize the array and its attributes
 * @param shared_data 
 * @return int error code
 */
int array_init(shared_data_t* shared_data);
/**
 * @brief make free to the array and its elements
 * @param shared_data 
 * @return int error code
 */
int array_uninit(shared_data_t* shared_data);
/**
 * @brief adds new element to the array
 * @param shared_data
 * @param file_path
 * @return int error code
 */
int array_append(shared_data_t* shared_data, char* file_path);

#endif  // ARRAY_H
