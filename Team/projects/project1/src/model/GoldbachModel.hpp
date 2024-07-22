// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef GOLDBACHMODEL_HPP
#define GOLDBACHMODEL_HPP

#include <vector>
#include "Assembler.hpp"
#include "Structs.hpp"


/// @brief  Class that calculates goldbach sums using "Criba de Eratostenes"
class GoldbachModel {
 public:
  /// Constructor
  GoldbachModel();
  /// Destructor
  ~GoldbachModel();
  /// @brief indicates if a numbers is in an array
  /// @param array array to search
  /// @param number  number to search
  /// @return true or false
  bool findNumber(std::vector<int64_t> array, int64_t number);
  /// @brief calculates all goldbach sums of numbers and stores it into
  /// an array
  /// @param input given array of numbers to calculate
  /// @param results given array to store the results
  /// @param sums given array to store total possible sums
  void calculateSums(int64_t input,
    std::vector<int64_t>& results, int64_t& sums);
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the sockets, meaning, client requests
  /// @return 1 on succesful execution
};

#endif  // GOLDBACHMODEL_HPP
