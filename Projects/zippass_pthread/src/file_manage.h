//  Copyright 2023 Sebastian Venegas

#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#include <stdbool.h>

#include <array.h>
#include <thread_manage.h>

enum ERRORS {
  INVALID_ARGUMENT,
  CANT_OPEN_ZIP,
  CANT_ALLOCATE_MEM,
  CANT_APPEND,
  ERROR_FILE_OPEN,
  ERR_CREATE_THREAD
};

/**
 * @brief starts process to find password
 * @param shared_data
 * @return error code
*/
int init(shared_data_t* shared_data);
/**
 * @brief tries to open a zip file with every possible variation of characters
 * @param data
 * @param iteration
 * @return error code
 */
int break_password(shared_data_t* data, size_t iteration);
/**
 * @brief tries to open a zip file with every possible variation of characters.
 * Function where threads are working
 * @param priv_data
 * @return error code
 */
void* found_pass(void* priv_data);
/**
 * @brief Receives private data where is apath to a zip file and a password.
 * Tries to open the zip file and the first archive in it with the password.
 * @param data
 * @return boolean, true if open successfully and false if not
*/
bool process_file(private_data_t* data);
/**
 * @brief reads from the standard input and stores it in the parameters passed
 * @param argc
 * @param argv
 * @param data
 * @return boolean, true if file can open and false otherwise
*/
int read_input(int argc, char* argv[], shared_data_t* data);

#endif  // FILE_MANAGE_H
