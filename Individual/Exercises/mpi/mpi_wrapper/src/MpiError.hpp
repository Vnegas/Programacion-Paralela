// Copyright 2023 Sebastian Venegas

#ifndef MPIERROR_HPP
#define MPIERROR_HPP

#include <stdexcept>
#include <iostream>

// Forward declaration of Mpi class
class Mpi;

class MpiError : public std::runtime_error {
 public:
  /**
   * @brief Constructor, inheriting from std::runtime_error
   * @param message
   *
   */
  explicit MpiError(const std::string& message) 
    : std::runtime_error(message){};
  /**
   * @brief Constructor, inheriting from std::runtime_error
   * @param message
   * @param mpi
   */
  MpiError(const std::string& message, const Mpi& mpi);
  /**
   * @brief Constructor, inheriting from std::runtime_error
   * @param message
   * @param mpi
   * @param theadNumber
   *
   */ 
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber); 
};

#endif // MPIERROR_HPP
