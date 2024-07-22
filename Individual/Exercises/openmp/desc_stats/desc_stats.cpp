// Copyright 2023 Sebastian Venegas

#include <omp.h>
#include <math.h>

#include <iostream>
#include <vector>

int main() {
  std::vector<double> values;

  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  if (values.size() == 0) {
    return 0;
  }

  double average = 0.0; double stdDev = 0.0;
  double min = values[0]; double max = 0.0;

  #pragma omp parallel default(none) shared(values, min, max, average, stdDev)
  {
    #pragma omp for reduction(+:average)
    for (size_t i = 0; i < values.size(); ++i) {
      if (min > values[i]) {
        min = values[i];
      }
      if (max < values[i]) {
        max = values[i];
      }
      average += values[i] / values.size();
    }

    #pragma omp for reduction(+:stdDev)
    for(size_t i = 0; i < values.size(); ++i) {
      stdDev += (values[i] - average) * (values[i] - average);
    }
  }

  stdDev = sqrt(stdDev / values.size());

  std::cout << "\nminimum: " << min << std::endl;
  std::cout << "average: " << average << std::endl;
  std::cout << "standard Deviation: " << stdDev << std::endl;
  std::cout << "maximum: " << max << std::endl;
}
