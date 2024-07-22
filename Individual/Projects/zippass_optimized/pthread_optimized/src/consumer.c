//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include "common.h"
#include "consumer.h"

void* consume(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = private_data->simulation;
  size_t file = 0;
  while (true) {
    // waits until theirs something in the queue to consume
    sem_wait(&simulation->can_consume);
    // consume
    queue_dequeue(&simulation->queue, private_data->password);
    // checks exit condition
    if (strcmp(private_data->password, "") == 0) {
      // all variations calculated, exit condition
      return NULL;
    }
    for (size_t i = file; i < simulation->file_count; ++i) {
      // checks flag if another thread already finished
      pthread_mutex_lock(&simulation->can_access_pass_found);
      // password for i file has already been found
      if (simulation->pass_found[i] == true) {
        pthread_mutex_unlock(&simulation->can_access_pass_found);
      } else {
        pthread_mutex_unlock(&simulation->can_access_pass_found);
        // check if password is correct
        // tries to open zip file
        if (process_file(private_data, i) == true) {
          // password is correct
          pthread_mutex_lock(&simulation->can_access_pass_found);
            simulation->pass_found[i] = true;
          pthread_mutex_unlock(&simulation->can_access_pass_found);
          // save password in conditionally safe
          snprintf(simulation->passwords[i]
            , strlen(private_data->password)+1, "%s", private_data->password);
        }
      }
    }
  }
  return NULL;
}

bool process_file(private_data_t* private_data, size_t file_num) {
  simulation_t* simulation = private_data->simulation;
  char* file_path = private_data->file[file_num];
  char* password = private_data->password;
  int error = 0;
  bool can_open = false;
  FILE* file = NULL;
  file = fopen(file_path, "r");
  if (!file) {
    fprintf(stderr, "cant open or read file\n");
    return ERROR_FILE_OPEN;
  }

  zip_t* zip_arch = NULL;
  // mutex to open zip file
  pthread_mutex_lock(&simulation->can_access_pass_found);
    zip_arch = zip_open(file_path, 0, &error);
  pthread_mutex_unlock(&simulation->can_access_pass_found);

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
