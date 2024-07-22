// Copyright 2023 Sebastian Venegas

#include <iostream>
#include <omp.h>
#include <random>

/**
 * @brief Generates pseudorandom numbers using a uniform distribution
 * @param amount
 * @param numbers
 */
void random_num(int amount, double* numbers);
/**
 * @brief Sorts array of numbers using the odd-even sort algorithm
 * @param n
 * @param arr
 * @param threadAmount
 */
void serial_odd_even_sort(size_t n, double* arr, int thread_count);
/**
 * @brief Changes the position of two numbers in an array
 * @param a
 * @param b
 */
void swap(double* a, double* b);
/**
 * @brief Prints an array of numbers
 * @param numbers
 * @param numberAmount
 */
void print(double* numbers, int number_count);

int main(int argc, char* argv[]) {
  int number_count = 0;
  int thread_count = omp_get_max_threads();

  if (argc >= 2) {
    number_count = atoi(argv[1]);
    if (argc == 3) {
      thread_count = atoi(argv[2]);
    }
    double* numbers = new double[number_count];
    random_num(number_count, numbers);
    print(numbers, number_count);

    serial_odd_even_sort(number_count, numbers, thread_count);

    std::cout << "\nOrdered ";
    print(numbers, number_count);
    delete[] numbers;
  } else {
    std::cerr << "Argument count error";
  }

  return 0;
}

void random_num(int amount, double* numbers) {
  for (int num = 0; num < amount; num++) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);
    numbers[num] = dist(rng);
  }
}

void serial_odd_even_sort(size_t n, double* arr, int thread_count) {
  bool is_sorted = false;
  while (!is_sorted) {
    is_sorted = true;
    #pragma omp parallel num_threads(thread_count) \
      default(none) shared(n, arr, is_sorted)
    {
      #pragma omp for
      for (size_t i = 1; i <= n - 2; i += 2) {
        if (arr[i] > arr[i + 1]) {
          #pragma omp critical (swap)
          swap(&arr[i], &arr[i + 1]);
          #pragma omp critical (swap)
          is_sorted = false;
        }
      }

      #pragma omp for
      for (size_t i = 0; i <= n - 2; i += 2) {
        if (arr[i] > arr[i + 1]) {
          #pragma omp critical (swap)
          swap(&arr[i], &arr[i + 1]);
          #pragma omp critical (swap)
          is_sorted = false;
        }
      }
    }
  }
}

void swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void print(double* numbers, int number_count) {
  #pragma omp barrier
  std::cout << "Numbers:" << std::endl;
  for (int num = 0; num < number_count; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}
