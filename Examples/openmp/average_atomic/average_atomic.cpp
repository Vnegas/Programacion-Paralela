#include <omp.h>

#include <iostream>
#include <vector>

void print_values(const char* schedule, const std::vector<int>& values);

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads ();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }
  (void) thread_count;

  std::vector<double> values;

  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double total_sum;

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values, total_sum)
  {
    double my_partial_sum = 0.0;
    #pragma omp for
    for (size_t i = 0; i < values.size(); ++i) {
      my_partial_sum += values[i];
    }
      // #pragma omp critical (can_sum)
      #pragma omp atomic
      total_sum += my_partial_sum;
  }

  const double average = values.size() ? total_sum / values.size() : 0.0;
  std::cout << average << std::endl;
}