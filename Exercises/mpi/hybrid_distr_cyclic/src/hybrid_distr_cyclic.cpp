// Copyright 2023 Sebastian Venegas

#include <cstdlib>
#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <iostream>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)

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

    int start = -1;  // Set start
    int finish = -1;  // Set finish
    std::vector<int> mapping;

    if (argc == 3) {  // Check if thread count was specified
      start = atoi(argv[1]);  // Set start
      finish = atoi(argv[2]);  // Set finish
    } else {
      if (process_number == 0) {
        std::cout << "Start: "; std::cin >> start;
        std::cout << "Finish: "; std::cin >> finish;
      }
    }

    if (MPI_Bcast(&start, 1, MPI_INT, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS) {
      fail("could not broadcast start");
    }
    if (MPI_Bcast(&finish, 1, MPI_INT, 0
        , MPI_COMM_WORLD) != MPI_SUCCESS ) {
      fail("could not broadcast finish");
    }
    // cyclic map
    for (int i = process_number; i < finish; i += process_count) {
      mapping.push_back(i);
    }
    // Calculate process size
    const int process_size = mapping.size();
    int my_start = -1; int my_finish = -1;
    for (int i = 0; i < process_size; ++i) {
      if (i == 0) {
        my_start = mapping[i];
      } else {
        my_finish = mapping[i];
      }
    }
    const double elapsed = MPI_Wtime() - start_time;
    std::cout << process_hostname << ':' << process_number + 1 << ": range [" 
      << my_start << ", " << my_finish << "[ size " << process_size
      << " in " << elapsed << "s" << std::endl;  // Print process range

    #pragma omp parallel default(none) \
      shared(std::cout, process_hostname, process_number, my_start, my_finish)
    {
      int thread_start = -1;  // Thread start
      int thread_finish = -1;  // Thread finish

      #pragma omp for schedule(static)
      for (int index = my_start; index < my_finish; ++index) {
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
    MPI_Finalize();
  }

  return 0;
}
