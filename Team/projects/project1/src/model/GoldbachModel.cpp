// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <stdlib.h>
#include <iostream>

#include "GoldbachModel.hpp"

GoldbachModel::GoldbachModel() {
}

GoldbachModel::~GoldbachModel() {
}

bool GoldbachModel::findNumber(std::vector<int64_t> array,
  int64_t number) {
  for (size_t i = 0; i < array.size(); i++) {
    if (array[i] == number) {
      return true;
    }
  }
  return false;
}

void GoldbachModel::calculateSums(int64_t input
    , std::vector<int64_t>& results, int64_t& sums) {
  if (input >= -5 && input <= 5) {
  } else {
    /// Get all primes from < numero.
    int numero = std::abs(input);

    /// Adapted from:
    /// https://www.lawebdelprogramador.com/foros/Dev-C/1534072
    /// -Criba-de-Eratostenes.html
    std::vector<bool> isPrime(numero, true);
    std::vector<int64_t> primes;

    isPrime[0] = false;
    isPrime[1] = false;

    for (int i = 2; i < numero; i++) {
      if (isPrime[i]) {
        primes.push_back(i);
        for (int j = 2; j * i < numero; j++) {
          isPrime[j * i] = 0;
        }
      }
    }

    /// Goldbach starts here.
    if (numero % 2 == 0) {
      for (size_t i = 0; i < primes.size(); i++) {
        int64_t resultado = numero - primes[i];
        if (findNumber(primes, resultado) && resultado >= primes[i]) {
          results.push_back(primes[i]);
          results.push_back(resultado);
          sums++;
        }
      }
    } else {
      for (size_t i = 0; i < primes.size(); i++) {
        for (size_t j = i; j < primes.size(); j++) {
          int64_t suma = primes[i] + primes[j];
          if (suma % 2 == 0) {
            int64_t resultado = numero - suma;
            if (findNumber(primes, resultado) && resultado >= primes[j]) {
              results.push_back(primes[i]);
              results.push_back(primes[j]);
              results.push_back(resultado);
              sums++;
            }
          }
        }  // end for size odd numbers.
      }  // end for size odd numbers.
    }  // end if number odd.
  }  // end if number > 5.
}  // end subrutine.
