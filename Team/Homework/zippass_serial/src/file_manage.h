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
 * @brief Receives a path to a zip file and a password. Tries to open the zip
 * file a the first archive in it with the password.
 * @param file_path
 * @param password
 * @return boolean, true if open successfully and false if not
*/
bool process_file(const char* file_path, const char* password);
/**
 * @brief reads from the standard input and stores it in the parameters passed
 * @param data
 * @return boolean, true if file can open and false otherwise
*/
int read_input(array_t* data);

#endif  // FILE_MANAGE_H
