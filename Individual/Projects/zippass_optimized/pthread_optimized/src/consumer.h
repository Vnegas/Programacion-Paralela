//  Copyright 2023 Sebastian Venegas

#ifndef CONSUMER_H
#define CONSUMER_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <zip.h>

/**
 * @brief tries to open zips files with every possible variation of characters
 * consumed
 * @param data
 */
void* consume(void* data);
/**
 * @brief Receives private data where is a path to a zip file, also receives
 * a possible password. Tries to open the zip file and the first archive in it.
 * @param private_data
 * @param file_num
 * @return boolean, true if open successfully and false if not
*/
bool process_file(private_data_t* private_data, size_t file_num);

#endif  // CONSUMER_H
