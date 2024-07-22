//  Copyright 2023 Sebastian Venegas

#ifndef THREAD_MANAGE_H
#define THREAD_MANAGE_H

#include <assert.h>
#include <dirent.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <array.h>
#include <file_manage.h>

/**
 * @brief creates new directory and copies the zip file located in the path
 * stored in data into the new directory
 * @param data
 * @param thread_num
 * @param file_num
*/
int copy_files(private_data_t* data, size_t thread_num, size_t file_num);
/**
 * @brief creates space in memory for private data and creates n threads
 * @param data
 * @param file_num
 * @param char_num
*/
int create_threads(shared_data_t* data, size_t file_num, size_t char_num);
/**
 * @brief creates space in memory for shared/public data
 * @param shared_data
*/
int create_shared_data(shared_data_t* shared_data);
/**
 * @brief calculate workspace
 * @param i
 * @param D
 * @param w
 * @return value of work (start or finish)
*/
size_t calculate_work(size_t i, size_t D, size_t w);
/**
 * @brief deletes an entire directory and the files inside it
 * @param path
*/
int remove_directory(const char* path);

#endif  // THREAD_MANAGE_H
