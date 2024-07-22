// Copyright 2023 Sebastian Venegas

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief Calculates the start index of a chunk of data for a given process
 * 
 * @param rank Process rank
 * @param end End index of data
 * @param workers Number of processes
 * @param begin Begin index of data
 * @return int 
 */
int calculate_start(int rank, int end, int workers, int begin);
/**
 * @brief Calculates the end index of a chunk of data for a given process
 * 
 * @param rank Process rank
 * @param end End index of data
 * @param workers Number of processes
 * @param begin Begin index of data
 * @return int 
 */
int calculate_finish(int rank, int end, int workers, int begin);

int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {  // Initialize MPI
    const double start_time = MPI_Wtime();

    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);  // Get process number

    int process_count = -1;  // Number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);  // Get process count

    char process_hostname[MPI_MAX_PROCESSOR_NAME];  // Hostname
    int hostname_length = -1;  // Hostname length
    // Get process hostname
    MPI_Get_processor_name(process_hostname, &hostname_length);

    int overall_start = -1;  // Set overall start
    int overall_finish = -1;  // Set overall finish

    if (argc == 3) {  // Check if thread count was specified
      overall_start = atoi(argv[1]);  // Set overall start
      overall_finish = atoi(argv[2]);  // Set overall finish
    } else {
      if (process_number == 0) {
        std::cout << "Start: "; std::cin >> overall_start;
        std::cout << "Finish: "; std::cin >> overall_finish;
      }
    }

    if (MPI_Bcast(&overall_start, 1, MPI_INT, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
      fail("could not broadcast overall start");
    }
    if (MPI_Bcast(&overall_finish, 1, MPI_INT, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      fail("could not broadcast overall finish");
    }

    const int process_start = calculate_start(process_number, overall_finish
      , process_count, overall_start);  // Calculate process start
    const int process_finish = calculate_finish(process_number, overall_finish
      , process_count, overall_start);  // Calculate process finish
    // Calculate process size
    const int process_size = process_finish - process_start;

    const double elapsed = MPI_Wtime() - start_time;
  
    std::cout << process_hostname << ':' << process_number + 1 << ": range [" 
      << process_start << ", " << process_finish << "[ size " << process_size
      << " in " << elapsed << "s" << std::endl;  // Print process range

    #pragma omp parallel default(none) \
      shared(std::cout, process_hostname, process_number, process_start \
      , process_finish)
    {
      int thread_start = -1;  // Thread start
      int thread_finish = -1;  // Thread finish

      #pragma omp for schedule(static)
      for (int index = process_start; index < process_finish; ++index) {
        if (thread_start == -1) {
          thread_start = index;  // Set thread start
        }
        thread_finish = index;  // Set thread finish
      }

      ++thread_finish;  // Increment thread finish
      // Calculate thread size
      const int thread_size = thread_finish - thread_start;

      #pragma omp critical(can_print)
      std::cout << '\t' << process_hostname << ':' << process_number + 1 << '.'
        << omp_get_thread_num() + 1 << ": range [" << thread_start << ", " <<
        thread_finish << "[ size " << thread_size << std::endl;
        // Print thread range on every process on every thread
    }
    MPI_Finalize();  // Finalize MPI
  }

  return 0;
}

int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;  // Calculate range
  // Calculate start
  return begin + rank * (range / workers) + std::min(rank , range % workers);
}

int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);  // Calculate finish
}
