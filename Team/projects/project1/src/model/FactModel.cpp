// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <math.h>

#include <stdlib.h>
#include <iostream>

#include "FactModel.hpp"

FactModel::FactModel() {}

FactModel::~FactModel() {}

void FactModel::prime_fact(int64_t input,
   std::vector<int64_t>& results) {
  if (input != 0) {
    while (input % 2 == 0) {
      results.push_back(2);
      input = input / 2;
    }

    // n must be odd at this point. So we can skip
    // one element (Note i = i +2)
    for (int i = 3; i <= sqrt(input); i = i + 2) {
      // While i divides n, print i and divide n
      while (input % i == 0) {
        results.push_back(i);
        input = input / i;
      }
    }
    // This condition is to handle the case when n
    // is a prime number greater than 2
    if (input > 2) {
      results.push_back(input);
    }
  } else {
    results.push_back(input);
  }
}
