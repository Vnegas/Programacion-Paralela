//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include "simulation.h"

/**
 * @brief reads from the standard input and stores it in the parameters passed
 * @param simulation
 * @param argc
 * @param argv
 * @return error code
*/
int analyze_arguments(simulation_t* simulation, int argc, char* argv[]);
/**
 * @brief Create consumers and producers
 * @param simulation
 * @return error code
 */
int create_consumers_producers(simulation_t* simulation);
/**
 * @brief waits every thread to finish its job 
 * @param count
 * @param threads
 * @return error code
 */
int join_threads(size_t count, pthread_t* threads);

simulation_t* simulation_create() {
  simulation_t* simulation = (simulation_t*) calloc(1, sizeof(simulation_t));
  if (simulation) {
    simulation->file_count = 0;
    simulation->consumer_count = 0;
    simulation->max_char_pass = 0;
    simulation->alphabet = (char*) calloc(100, sizeof(char));
    simulation->files = (char**) calloc(100, sizeof(char));

    queue_init(&simulation->queue);
    pthread_mutex_init(&simulation->can_access_pass_found, NULL);
    sem_init(&simulation->can_consume, 0, 0);
  }
  return simulation;
}

private_data_t* create_private_data(simulation_t* simulation) {
  assert(simulation);
  // reserve memory for private data
  private_data_t* private_data = (private_data_t*)
    calloc(simulation->consumer_count, sizeof(private_data_t));

  if (private_data) {
    // assign memory and create memory
    for (size_t thread_number = 0; thread_number < simulation->consumer_count
        ; ++thread_number) {
      // assign public data
      private_data[thread_number].simulation = simulation;
      private_data[thread_number].password = (char*)
        calloc(simulation->max_char_pass * 2, sizeof(char));
      // memory for files (copy created)
      private_data[thread_number].file = (char**)
        calloc(simulation->file_count, sizeof(char*));
      if ((!private_data[thread_number].password)
          || (!private_data[thread_number].file)) {
        return NULL;
      }

      for (size_t file_num = 0; file_num < simulation->file_count; ++file_num) {
        private_data[thread_number].file[file_num] = (char*)
          calloc(100, sizeof(char));
        if (!private_data[thread_number].file[file_num]) {
          return NULL;
        }
      }
    }

    // copy files
    for (size_t thread_number = 0; thread_number < simulation->consumer_count
        ; ++thread_number) {
      for (size_t file_num = 0; file_num < simulation->file_count; ++file_num) {
        copy_files(private_data, file_num, thread_number);
      }
    }

    return private_data;
  }
  return NULL;
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  // free memory allocated
  for (size_t i = 0; i < simulation->file_count; ++i) {
    free(simulation->files[i]);
    free(simulation->passwords[i]);
  }
  free(simulation->pass_found);
  free(simulation->files);
  free(simulation->passwords);
  free(simulation->alphabet);
  // blanck attributes
  simulation->consumer_count = simulation->max_char_pass = 0;
  simulation->file_count = 0;
  simulation->pass_found = false;
  // destroy mutex, sem and queue
  sem_destroy(&simulation->can_consume);
  pthread_mutex_destroy(&simulation->can_access_pass_found);
  queue_destroy(&simulation->queue);
  free(simulation);
}

void private_data_destroy(private_data_t* private_data
    , simulation_t* simulation) {
  // free memory allocated
  for (size_t thread_number = 0; thread_number < simulation->consumer_count
      ; ++thread_number) {
    for (size_t file_num = 0; file_num < simulation->file_count; ++file_num) {
      free(private_data[thread_number].file[file_num]);
    }
    free(private_data[thread_number].file);
    free(private_data[thread_number].password);
  }
  // blanck attributes
  private_data->simulation = NULL;
  free(private_data);
}

int simulation_run(simulation_t* simulation, int argc, char* argv[]) {
  int error = analyze_arguments(simulation, argc, argv);
  if (error == EXIT_SUCCESS) {
    struct timespec start_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

    error = create_consumers_producers(simulation);

    struct timespec finish_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

    double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("execution time: %.9lfs\n", elapsed);
  }
  return error;
}

int analyze_arguments(simulation_t* simulation, int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (argc == 2) {
    if (sscanf(argv[1], "%zu", &simulation->consumer_count) != 1
      || simulation->consumer_count == 0) {
        fprintf(stderr, "error: invalid unit count\n");
        error = INVALID_ARGUMENT;
    }
  } else {
    simulation->consumer_count = sysconf(_SC_NPROCESSORS_ONLN);
  }

  // reads possibles characters and max characters
  scanf("%s %zu", simulation->alphabet, &simulation->max_char_pass);
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
      if (append(simulation, file_path) == EXIT_FAILURE) {
        fprintf(stderr, "can not append new file\n");
        error = CANT_APPEND;
      }
    }
    simulation->pass_found = (bool*)
    calloc(simulation->file_count, sizeof(bool));
    // stores passwords for each file, conditionally safe
    simulation->passwords = (char**)
      calloc(simulation->file_count, sizeof(char*));
    for (size_t index = 0; index < simulation->file_count; ++index) {
      simulation->passwords[index] = (char*)
        calloc (simulation->max_char_pass * 2, sizeof(char));
    }
    free(file_path);
  }
  return error;
}

int join_threads(size_t count, pthread_t* threads) {
  int error = EXIT_SUCCESS;
  for (size_t i = 0; i < count; ++i) {
    // todo: sum could not be right
    error += pthread_join(threads[i], /*value_ptr*/ NULL);
  }
  free(threads);
  return error;
}

int create_consumers_producers(simulation_t* simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;
  private_data_t* private_data = create_private_data(simulation);
  // create threads
  pthread_t producer;
  pthread_t* consumers = (pthread_t*) calloc(simulation->consumer_count
    , sizeof(pthread_t));
  if (pthread_create(&producer, /*attr*/ NULL, produce, simulation)
      != EXIT_SUCCESS) {
    fprintf(stderr, "error: could not create producer thread\n");
    error += pthread_join(producer, /*value_ptr*/ NULL);
    return error;
  }
  if (consumers) {
    for (size_t i = 0; i < simulation->consumer_count; ++i) {
      if (pthread_create(&consumers[i], NULL, consume, &private_data[i])
          != EXIT_SUCCESS) {
        fprintf(stderr, "error: could not create consumer thread %zu\n", i);
        error += join_threads(i, consumers);
        return error;
      }
    }
  }
  // wait to finish (join)
  if (consumers) {
    error += pthread_join(producer, /*value_ptr*/ NULL);
    join_threads(simulation->consumer_count, consumers);
    // print passwords
    printf("\n");
    for (size_t i = 0; i < simulation->file_count; ++i) {
      if (simulation->pass_found[i]) {
        printf("%s %s\n", simulation->files[i], simulation->passwords[i]);
      } else {
        printf("%s\n", simulation->files[i]);
      }
    }
  } else {
    fprintf(stderr, "error: could not create threads\n");
    error = ERR_CREATE_THREAD;
  }
  private_data_destroy(private_data, simulation);
  return error;
}

int append(simulation_t* simulation, char* file_path) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  simulation->files[simulation->file_count] = (char*) calloc(300, sizeof(char));
  if (!simulation->files[simulation->file_count]) {
    error = EXIT_FAILURE;
  } else {
    strcpy(simulation->files[simulation->file_count], file_path);
  }

  if (error == EXIT_SUCCESS) {
    ++simulation->file_count;
  }
  return error;
}

// inspired/taken from: https://acortar.link/i2pKg8
void rand_str(char *dest, size_t length) {
  char charset[] = "0123456789"
                   "abcdefghijklmnopqrstuvwxyz"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  while (length-- > 0) {
    size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}

int copy_files(private_data_t* private_data, size_t file_num
    , size_t thread_num) {
  FILE* original_file; FILE* copy_file;
  char* original_filename = private_data->simulation->files[file_num];
  // random name for temp file
  char copy_filename[] = { [41] = '\1' };
  rand_str(copy_filename, sizeof copy_filename - 1);
  assert(copy_filename[41] == '\0');
  // put in directory wanted and .zip format
  copy_filename[0] = 't'; copy_filename[1] = 'e'; copy_filename[2] = 'm';
  copy_filename[3] = 'p'; copy_filename[4] = '/'; copy_filename[37] = '.';
  copy_filename[38] = 'z'; copy_filename[39] = 'i'; copy_filename[40] = 'p';

  char buffer[1024];
  size_t bytesRead;

  original_file = fopen(original_filename, "rb");
  if (original_file == NULL) {
    fprintf(stderr, "ERR_CANT_OPEN_FILE\n");
    return ERROR_FILE_OPEN;
  }

  copy_file = fopen(copy_filename, "wb");
  if (copy_file == NULL) {
    fprintf(stderr, "ERR_CANT_OPEN_FILE\n");
    fclose(original_file);
    return ERROR_FILE_OPEN;
  }

  // read original private_data and copy it
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), original_file)) > 0) {
    fwrite(buffer, 1, bytesRead, copy_file);
  }

  // assign temp file to thread;
  strcpy(private_data[thread_num].file[file_num], copy_filename);

  // Close files
  fclose(original_file);
  fclose(copy_file);
  return EXIT_SUCCESS;
}

// inspired/taken from: https://rb.gy/a9y1r
int remove_directory(const char *path) {
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int r = -1;

  if (d) {
    struct dirent *p;
    r = 0;
    while (!r && (p=readdir(d))) {
      int r2 = -1;
      char *buf;
      size_t len;
      /* Skip the names "." and ".." as we don't want to recurse on them. */
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        continue;

      len = path_len + strlen(p->d_name) + 2;
      buf = malloc(len);
      if (buf) {
        struct stat statbuf;

        snprintf(buf, len, "%s/%s", path, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode))
            r2 = remove_directory(buf);
          else
            r2 = unlink(buf);
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }
  if (!r)
    r = rmdir(path);

  return r;
}
