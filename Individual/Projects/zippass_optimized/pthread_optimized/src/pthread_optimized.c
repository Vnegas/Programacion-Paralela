// Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "simulation.h"

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  simulation_t* simulation = simulation_create();
  if (simulation) {
    // create temporary directory
    int error = mkdir("./temp", 0777);
    if (error == EXIT_FAILURE) {
      return error;
    }
    error = simulation_run(simulation, argc, argv);
    simulation_destroy(simulation);
    remove_directory("./temp");
  } else {
    fprintf(stderr, "error: could not allocate simulation\n");
    error = EXIT_FAILURE;
  }
  return error;
}
