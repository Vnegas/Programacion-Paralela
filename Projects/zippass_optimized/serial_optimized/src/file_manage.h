//  Copyright 2023 Sebastian Venegas

#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

enum ERRORS {
  INVALID_ARGUMENT,
  CANT_OPEN_ZIP,
  CANT_ALLOCATE_MEM,
  CANT_APPEND,
  ERROR_FILE_OPEN
};

/**
 * @brief tries to open a zip file with every possible variation of characters
 * @param data
 * @param index
 * @return error code
 */
int break_password(array_t* data, size_t index);
/**
 * @brief starts the process to find passwords
 * @param data
 */
void init(array_t* data);
/**
 * @brief reads from the standard input and stores it in the parameters passed
 * @param data
 * @return boolean, true if file can open and false otherwise
*/
int read_input(array_t* data);

#endif  // FILE_MANAGE_H
