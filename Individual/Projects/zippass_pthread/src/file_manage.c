//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <zip.h>

#include <file_manage.h>

/**
 * @brief changes a decimal number to a number in the given base
 * @param data
 * @param base
 * @param number
*/
void change_base(private_data_t* data, const size_t base, int number);

int read_input(int argc, char* argv[], shared_data_t* data) {
  int error = EXIT_SUCCESS;
  // read argument, thread count
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &data->thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid number\n");
      return INVALID_ARGUMENT;
    }
  } else {
    data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  }

  // reads possibles characters and max characters
  scanf("%s %zu", data->alphabet, &data->max_char_pass);
  // ++data->max_char_pass;
  char* file_path = (char*) calloc(300, sizeof(char));
  if (file_path) {
    for (size_t i = 0; i < 50; ++i) {
      // reads the path to the zip file to open
      // if it reaches EOF it stops reading
      if (scanf("%s", file_path) == EOF) {
        break;
      }
      // creates memory for the path and stores it
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

int init(shared_data_t* shared_data) {
  int error = EXIT_FAILURE;
  error = create_shared_data(shared_data);
  // search passwords
  for (size_t i = 0; i < shared_data->count; ++i) {
    error = break_password(shared_data, i);
  }
  // delete/destroy mutex
  pthread_mutex_destroy(&shared_data->can_access);
  // print passwords
  printf("\n");
  for (size_t i = 0; i < shared_data->count; ++i) {
    if (shared_data->pass_found[i]) {
      printf("%s %s\n", shared_data->files[i], shared_data->passwords[i]);
    } else {
      printf("%s\n", shared_data->files[i]);
    }
  }
  return error;
}

int break_password(shared_data_t* data, size_t iteration) {
  int error = EXIT_FAILURE;
  for (size_t i = 0; i < data->max_char_pass + 1; ++i) {
    // creation of threads
    error = create_threads(data, iteration, i);
  }
  return error;
}  // end break_password

void* found_pass(void* priv_data) {
  private_data_t* private_data = (private_data_t*) priv_data;
  shared_data_t* data = private_data->shared_data;

  size_t characters_count = strlen(data->alphabet);
  size_t workspace = pow(characters_count, private_data->char_num);
  // assign work
  private_data->start_work = calculate_work(private_data->thread_id, workspace
    , data->thread_count);
  private_data->finish_work = calculate_work(private_data->thread_id + 1
    , workspace, data->thread_count);
  for (size_t j = private_data->start_work; j <= private_data->finish_work
      ; ++j) {
    // tobase and recieve it
    change_base(private_data, characters_count, j);
    // try variation on password
    for (size_t k = 0; k < private_data->size_index_pass; ++k) {
      private_data->password[k] = data->alphabet[private_data->index_pass[k]];
    }
    // checks flag if another thread already finished
    pthread_mutex_lock(&data->can_access);
      bool is_finished = data->pass_found[private_data->file_num];
    pthread_mutex_unlock(&data->can_access);
    if (is_finished) {
      return EXIT_SUCCESS;
    }
    // check if password is correct
    // tries to open zip file
    if (process_file(private_data) == true) {
      pthread_mutex_lock(&data->can_access);
        data->pass_found[private_data->file_num] = true;
      pthread_mutex_unlock(&data->can_access);
      snprintf(data->passwords[private_data->file_num], private_data->char_num+2
        , "%s", private_data->password);
      return EXIT_SUCCESS;
    }

    if (private_data->size_index_pass == 1) {
      private_data->index_pass[private_data->size_index_pass] = 0;
    } else {
      private_data->index_pass[private_data->size_index_pass-1] = 0;
    }
    // try variation on password
    for (size_t k = 0; k < private_data->size_index_pass; ++k) {
      private_data->password[k] = data->alphabet[private_data->index_pass[k]];
    }
    // checks flag if another thread already finished
    if (process_file(private_data) == true) {
      pthread_mutex_lock(&data->can_access);
        data->pass_found[private_data->file_num] = true;
      pthread_mutex_unlock(&data->can_access);
      snprintf(data->passwords[private_data->file_num], private_data->char_num+2
        , "%s", private_data->password);
      return EXIT_SUCCESS;
    }
  }
  return NULL;
}

// TODO(me): fix 01 or 001 or 010 or 033 and so on are left out
void change_base(private_data_t* private_data, const size_t base, int number) {
  size_t index = 0;  // Initialize index of result
  // Convert input number is given base by repeatedly
  // dividing it by base and taking remainder
  while (number > 0) {
    private_data->index_pass[index] = (number % base);
    number /= base;
    ++index;
  }

  if (index == 0) {
    private_data->size_index_pass = 1;
  } else {
    private_data->size_index_pass = index;
  }
}

bool process_file(private_data_t* priv_data) {
  shared_data_t* data = priv_data->shared_data;
  char* file_path = priv_data->file;
  char* password = priv_data->password;
  int error = 0;
  bool can_open = false;
  // printf("%s\t%s\n", file_path, password);
  // if file cant be open or read, stop program
  FILE* file = NULL;
  file = fopen(file_path, "r");
  if (!file) {
    fprintf(stderr, "cant open or read file\n");
    return ERROR_FILE_OPEN;
  }

  zip_t* zip_arch = NULL;
  // mutex to open zip file
  pthread_mutex_lock(&data->can_access);
    zip_arch = zip_open(file_path, 0, &error);
  pthread_mutex_unlock(&data->can_access);

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
