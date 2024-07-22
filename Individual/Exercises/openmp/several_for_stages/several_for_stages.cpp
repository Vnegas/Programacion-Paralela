// Copyright 2023 Sebastian Venegas

#include <omp.h>

#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << std::endl;
  int thread_count = omp_get_max_threads ();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int stage_count = 3;
  if (argc == 4) {
    stage_count = atoi(argv[3]);
  }

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout, stage_count)
  {
    for (int stage_num = 0; stage_num < stage_count; ++stage_num) {
      #pragma omp for
      for (int i = 0; i < iteration_count; ++i) {
        #pragma omp critical (stdout)
          std::cout << "stage " << stage_num + 1 << ": " 
            << omp_get_thread_num() + 1 << '/' << omp_get_num_threads()
            << ": iteration " << i + 1 << '/' << iteration_count << std::endl;
      }

      #pragma omp single
      std::cout << std::endl;
    }
  }
}
