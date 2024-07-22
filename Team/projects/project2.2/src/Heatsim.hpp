// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef HEATSIM_HPP
#define HEATSIM_HPP

#include <omp.h>
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <exception>

#include "Common.hpp"

/// Main class to simulate heat transfer
class HeatSim {
 public:
  /// Vector that contains the data from txt file
  std::vector<std::string> info;
  /// Vector that separates each data into the corresponding job
  std::vector<data_t> jobs;
  /// Path of the file
  std::string path;
  /// Full extension of the path without file extension
  std::string path_no_extension;

  /// Number of threads
  int64_t num_threads = std::thread::hardware_concurrency();
  /// @brief Default constructor
  HeatSim() {}
  /// @brief Function to analize the arguments and store the needed information
  /// @param argv arguments
  void analyze_arguments(char *argv[]);
  /// @brief Extracts data from info vector to separate it in another vector
  void extract_data(std::string txt);
  /// @brief Extracts data from binary file to create and fill a matrix
  void fill_matrix();
  /// @brief Simulate heat transimition
  void transfer_heat(int index);
  /// @brief Simulate the state change of a celd
  /// @param index index of job we are working with
  bool state_change(int index);
  /// @brief Calculates new temperature of a celd
  /// @param index index of job we are working with
  /// @param row
  /// @param col
  double calculate_temp(int index, int row, int col);
  /// @brief Writes report in a tsv file of the results of the simulation
  /// @return string with formated elapsed
  std::string write_report();
  /// @brief Writes matrix into a binary file
  void write_binary_matrix();
  /// @brief Gives elapsed time appropietaded format
  /// @param seconds elapsed time
  /// @return time
  static std::string format_time(const time_t seconds);
  /// @brief Destructor
  ~HeatSim(){}
};

#endif  // HEADSIM_HPP
