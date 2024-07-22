#include <omp.h>

#include <iostream>
#include <vector>

void print_mapping(const char* schedule, const std::vector<int>& mapping);

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads ();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int block_size = 0;
  if (argc == 4) {
    block_size = atoi(argv[3]);
  }

  std::cout << std::endl << "          ";
  for (int i = 0; i < iteration_count; ++i) {
    std::cout << i << (i == iteration_count - 1 ? "\n" : " ");
  }

  std::vector<int> mapping(iteration_count);

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, mapping, block_size)
  {
      if (block_size == 0) {
      #pragma omp for schedule(static)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("static    ", mapping);

      #pragma omp for schedule(dynamic)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic   ", mapping);

      #pragma omp for schedule(guided)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided    ", mapping);
    } else {
      #pragma omp for schedule(static, block_size)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("static    ", mapping);

      #pragma omp for schedule(dynamic, block_size)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic   ", mapping);

      #pragma omp for schedule(guided, block_size)
      for (int i = 0; i < iteration_count; ++i) {
        mapping[i] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided    ", mapping);
    }
  }
}

void print_mapping(const char* schedule, const std::vector<int>& mapping) {
  std::cout << schedule;
  for (size_t i = 0; i < mapping.size(); ++i) {
    if (i < 9) {
      std::cout << mapping[i] << " ";
    } else {
      std::cout << mapping[i] << "  ";
    }
  }
  std::cout << std::endl;
}
