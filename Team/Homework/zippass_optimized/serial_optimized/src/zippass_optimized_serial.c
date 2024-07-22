//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zip.h>

#include <array.h>
#include <file_manage.h>

int main(void) {
  int error = EXIT_SUCCESS;
  // creates memory for passwords and path of zip files
  array_t data;
  if (array_init(&data) == EXIT_SUCCESS) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = read_input(&data);
    printf("\n");
    init(&data);
    struct timespec finish_time;
    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_seconds = finish_time.tv_sec - start_time.tv_sec
      + 1e-9 * (finish_time.tv_nsec - start_time.tv_nsec);
    fprintf(stderr, "\nExecution time %.9lfs\n", elapsed_seconds);

    array_uninit(&data);
  }
  return error;
}  // end
