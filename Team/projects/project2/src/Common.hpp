// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>

/// @brief Struct to store each parameter of the differente jobs
typedef struct data {
  std::vector <std::vector<double>> current_plate;
  std::string plate_name;
  double delta_time;
  double diffusion;
  double dimensions;
  double epsilon;
  std::vector <std::vector<double>> temp_plate;
  int k;
  size_t rows;
  size_t cols;
  double time_elapsed;
} data_t;

#endif  // COMMON_HPP
