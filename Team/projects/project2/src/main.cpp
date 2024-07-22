// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include "Heatsim.hpp"

int main(int argc, char *argv[]) {
  int error = EXIT_SUCCESS;
  if (argc >= 2) {
    HeatSim obj;
    // Si se especificaron cantidad de hilos
    if (argc == 3) {
      obj.num_threads = std::stoi(argv[2]);
    }
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    obj.run(argv);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);

    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("Execution time: %.9lfs\n", elapsed_time);

  } else {
    throw std::runtime_error("Must provide a valid file name");
    error = EXIT_FAILURE;
  }
  return error;
}
