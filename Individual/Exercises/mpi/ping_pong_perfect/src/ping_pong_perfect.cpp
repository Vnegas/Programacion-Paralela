// Copyright 2023 Sebastian Venegas

#include "Mpi.hpp"
#include "MpiError.hpp"
#include <iostream>
#include <stdexcept>

void serve(Mpi& mpi, int process_number, int process_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  try {
    Mpi mpi(argc, argv);
    int process_number = mpi.getProcessNumber();
    int process_count = mpi.getProcessCount();
    if (process_count != 2) {
      throw MpiError("Invalid number of process");
    }
    serve(mpi, process_number, process_count);
  } catch (const MpiError& errorMpi) {
    std::cerr << "error: " << errorMpi.what() << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void serve(Mpi& mpi, int process_number, int process_count) {
  int signal = 1;
  const int previous_process = (process_count + process_number + 1)
    % process_count; // previous process
  const int next_process = (process_number + 1) % process_count;
  int serve_count = 0;
  #pragma omp parallel num_threads(process_count) default(none) \
    shared(mpi, std::cout, std::cerr, process_number, process_count, previous_process, next_process, signal) \
    private(serve_count)
  {
    while (true) {
      if (process_number == 0) {
        mpi.send(signal, next_process, 0);
        #pragma omp critical(stdout)
        std::cout << "Process " << process_number + 1 << ": " << serve_count++
          << " serves" << std::endl;
        mpi.receive(signal, previous_process);
      } else {
        mpi.receive(signal, previous_process);
        #pragma omp critical(stdout)
        std::cout << "Process " << process_number + 1 << ": " << serve_count++
          << " serves" << std::endl;
        mpi.send(signal, next_process, 0);
      }
    }
  }
}
