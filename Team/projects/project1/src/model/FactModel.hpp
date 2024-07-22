// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef FACTMODEL_HPP
#define FACTMODEL_HPP

#include <vector>
#include "Assembler.hpp"
#include "Structs.hpp"


/// @brief  Class that calculates prime factorization of numbers
class FactModel {
 public:
  /// Constructor
  FactModel();
  /// Destructor
  ~FactModel();
  /// @brief Calculates primer factorization of a numbers and
  /// stores the results in a vector
  /// taken from https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
  void prime_fact(int64_t input, std::vector<int64_t>& results);
};

#endif  // FACTMODEL_HPP

