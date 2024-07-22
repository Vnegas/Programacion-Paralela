//  Copyright 2023 Sebastian Venegas

#ifndef COMMON_H
#define COMMON_H

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

enum {
  ERR_CREATE_THREAD,
  INVALID_ARGUMENT,
  CANT_OPEN_ZIP,
  CANT_ALLOCATE_MEM,
  CANT_APPEND,
  ERROR_FILE_OPEN,
};

/**
 * @brief struct that acts like a dynamic array, contains the count of files,
 * the number of threads, the number of max character to use in the password,
 * paths of files, the alphabet, a queue, the passwords
 */
typedef struct simulation {
  size_t file_count;
  size_t consumer_count;
  size_t max_char_pass;
  char* alphabet;
  char** files;

  queue_t queue;
  pthread_mutex_t can_access_pass_found;
  bool* pass_found;
  char** passwords;  // conditionally safe, where to print
  sem_t can_consume;
} simulation_t;

/**
 * @brief struct that contains a possible password, paths to the zip file and
 * a pointer to the shared data (simulation)
 * that zip file
 */
typedef struct private_data {
  char** file;
  char* password;
  size_t char_num;
  simulation_t* simulation;
} private_data_t;

#endif  // COMMON_H
