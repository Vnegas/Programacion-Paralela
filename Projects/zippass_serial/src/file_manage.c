//  Copyright 2023 Sebastian Venegas

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

#include <array.h>
#include <file_manage.h>

/**
 * @brief changes a decimal number to a number in the given base
 * @param data
 * @param base
 * @param number
*/
void change_base(array_t* data, const size_t base, size_t number);

int read_input(array_t* data) {
  int error = EXIT_SUCCESS;
  // reads possibles characters and max characters
  scanf("%s %zu", data->alphabet, &data->max_char_pass);
  char* file_path = (char*) calloc(300, sizeof(char));
  if (file_path) {
    for (size_t i = 0; i < 50; ++i) {
      // reads the path to the zip file to open
      // if it reaches EOF it stops reading
      if (scanf("%s", file_path) == EOF) {
        break;
      }
      // creates memory for the path and stores it. Creates memory for password
      if (array_append(data, file_path) == EXIT_FAILURE) {
        fprintf(stderr, "can not append new file\n");
        error = CANT_APPEND;
      }
    }
    free(file_path);
  } else {
    // error
  }
  return error;
}

int break_password(array_t* data, size_t index) {
  int error = EXIT_FAILURE;
  size_t characters_count = strlen(data->alphabet);
  for (size_t i = 1; i < data->max_char_pass+1; ++i) {
    size_t max_poss = pow(characters_count, i);
    for (size_t j = 0; j < max_poss; ++j) {
      // tobase and recieve it
      change_base(data, characters_count, j);
      // try variation on password
      for (size_t k = 0; k < data->elements->size_index_pass; ++k) {
        data->elements[index].password[k] = data->alphabet[data
          ->elements->index_pass[k]];
      }
      // check if password is correct
      // tries to open zip file
      if (process_file(data->elements[index].file, data->elements[index].
          password) == true) {
        printf("%s %s\n", data->elements[index].file, data->elements[index].
          password);
        return EXIT_SUCCESS;
      }

      if (data->elements->size_index_pass == 1) {
        data->elements->index_pass[data->elements->size_index_pass] = 0;
      } else {
        data->elements->index_pass[data->elements->size_index_pass-1] = 0;
      }
      // try variation on password
      for (size_t k = 0; k < data->elements->size_index_pass; ++k) {
        data->elements[index].password[k] = data->alphabet[data
          ->elements->index_pass[k]];
      }
      if (process_file(data->elements[index].file, data->elements[index].
          password) == true) {
        printf("%s %s\n", data->elements[index].file, data->elements[index].
          password);
        return EXIT_SUCCESS;
      }
    }
  }
  return error;
}  // end break_password

void change_base(array_t* data, const size_t base, size_t number) {
  size_t index = 0;  // Initialize index of result
  // Convert input number is given base by repeatedly
  // dividing it by base and taking remainder
  while (number > 0) {
    data->elements->index_pass[index] = (number % base);
    number /= base;
    ++index;
  }

  if (index == 0) {
    data->elements->size_index_pass = 1;
  } else {
    data->elements->size_index_pass = index;
  }
}

bool process_file(const char* file_path, const char* password) {
  int error = 0;
  bool can_open = false;
  // if file cant be open or read, stop program
  FILE* file = NULL;
  file = fopen(file_path, "r");
  if (!file) {
    fprintf(stderr, "cant open or read file\n");
    return ERROR_FILE_OPEN;
  }
  zip_t* zip_arch = NULL;
  zip_arch = zip_open(file_path, 0, &error);
  // struct that contains info about the zip file
  struct zip_stat* zip_info = NULL;
  // to allocate space, just enough
  zip_info = calloc(256, sizeof(int));
  zip_stat_init(zip_info);

  zip_file_t* file_d = NULL;
  char* txt = NULL;

  zip_stat_index(zip_arch, 0, 0, zip_info);
  txt = calloc(zip_info->size + 1, sizeof(char));
  file_d = zip_fopen_index_encrypted(zip_arch, 0, 0, password);
  if (file_d) {
    zip_fread(file_d, txt, zip_info->size);
    if (strcmp(txt, "CI0117-23a") == 0) {
      can_open = true;
    }
    zip_fclose(file_d);
  }

  free(txt);
  free(zip_info);
  zip_close(zip_arch);
  fclose(file);
  return can_open;
}
