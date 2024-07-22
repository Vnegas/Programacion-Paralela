#include <omp.h>

#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << std::endl;
  int thread_count = omp_get_max_threads ();
  if (argc == 2) {
    thread_count = atoi(argv[1]);
  }

  #pragma omp parallel num_threads(thread_count)
  {
    #pragma omp critical (stdout)
      std::cout << "Hello from secondary thread " << omp_get_thread_num() + 1
        << " of " << omp_get_num_threads() << std::endl;
  }
}

#if 0
  #pragma omp parallel num_threads(2)
  {
    if (omp_get_thread_num() == 0) {
      produce();
    } else {
      consume();
    }
  }
#endif