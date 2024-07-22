// Copyright 2023 Sebastian Venegas

#include <iostream>
#include <random>

/**
 * @brief Generates pseudorandom numbers using a uniform distribution
 * @param amount
 * @param numbers
 */
void randomNum(int amount, double* numbers);
/**
 * @brief Sorts array of numbers using the odd-even sort algorithm
 * @param n
 * @param arr
 * @param threadAmount
 */
void serialOddEvenSort(size_t n, double* arr);
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
  if (argc >= 2) {
    number_count = atoi(argv[1]);
    double* numbers = new double[number_count];
    randomNum(number_count, numbers);
    print(numbers, number_count);

    serialOddEvenSort(number_count, numbers);

    std::cout << "\nOrdered ";
    print(numbers, number_count);
    delete[] numbers;
  } else {
    std::cerr << "Argument count error";
  }

  return 0;
}

void randomNum(int amount, double* numbers) {
  for (int num = 0; num < amount; num++) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);
    numbers[num] = dist(rng);
  }
}

void serialOddEvenSort(size_t n, double* arr) {
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (size_t i = 1; i <= n - 2; i += 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }
    for (size_t i = 0; i <= n - 2; i += 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
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
  std::cout << "Numbers:" << std::endl;
  for (int num = 0; num < number_count; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}
