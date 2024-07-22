// Copyright 2023 Sebastian Venegas

#ifndef MPI_WRAPPER_MPI_H
#define MPI_WRAPPER_MPI_H

#include "mpi.h"

#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class MpiError;

/**
 * @brief A wrapper for the MPI library.
 * @attribute: processCount The number of processes in the MPI world.
 * @attribute: processNumber The rank of the current process in the MPI world.
 * @attribute: hostname The hostname of the current process.
 */
class Mpi {
 private:
  int processNumber;
  int processCount;
  std::string hostname;

 public:
  /**
   * @brief Construct a new Mpi wrapper object
   * @param argc
   * @param argv
   *
   */
  Mpi(int &argc, char** &argv);
  /**
   * @brief Destroy the Mpi object  
   */
  ~Mpi();
  /**
   * @brief Get the Process Number MPI
   * @return process number
   */
  int getProcessNumber() const;
  /**
   * @brief Get the Process Count MPI
   * @return process count
   */
  int getProcessCount() const;
  /**
   * @brief Get the Hostname MPI
   * @return hostname
   */
  std::string getHostname() const;

  // send
  /**
   * @brief Send a text to another process
   * @param text
   * @param toProcess
   * @param tag
   */
  void send(const std::string& text, int toProcess, int tag = 0);
  // template for send
  template <typename DataType>
  /**
   * @brief Send a scalar value to another process
   * @param DataType
   * @param toProcess
   * @param tag
   */
  void send(const DataType& value, int toProcess, int tag = 0) {
    if (MPI_Send(&value, 1, map(value), toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("MPI_Send failed");
    }
  }
  // template for send
  template <typename DataType>
  /**
   * @brief Send an array of count values to another process
   * @param values
   * @param count
   * @param toProcess
   * @param tag
   */
  void send(const DataType* values, int count, int toProcess, int tag = 0) {
    if (MPI_Send(values, count, map(values[0]), toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("MPI_Send failed");
    }
  }
  // template for send
  template <typename DataType>
  /**
   * @brief Send an array of count values to another process
   * @param values
   * @param toProcess
   * @param tag
   */
  void send(const std::vector<DataType>& values, int toProcess, int tag = 0) {
    if (MPI_Send(values.data(), values.size(), map(values[0]), toProcess, tag,
        MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("MPI_Send failed");
    }
  }

  // receive
  /**
    * @brief Wait until it receives a text of at most length chars from
    * another process
    * @param text
    * @param capacity
    * @param fromProcess
    * @param tag
    */
  void receive(std::string& text, int capacity, int fromProcess = MPI_ANY_SOURCE
    , int tag = MPI_ANY_TAG);
  // template for receive
  template <typename DataType>
  /**
   * @brief Wait until it receives a scalar value from other process
   * @param value
   * @param fromProcess
   * @param tag
   */
  void receive(DataType& value, int fromProcess = MPI_ANY_SOURCE
      , int tag = MPI_ANY_TAG) {
    if(MPI_Recv(&value, 1, map(value), fromProcess, tag
        , MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw MpiError("MPI_Recv failed");
    }
  }
  // template for receive
  template <typename DataType>
  /**
   * @brief Wait until it receives at most capacity values from another process
   * @param values
   * @param capacity
   * @param fromProcess
   * @param tag
   */
  void receive(DataType* values, int capacity, int fromProcess = MPI_ANY_SOURCE,
      int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values, capacity, map(values[0]), fromProcess, tag,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw MpiError("MPI_Recv failed");
    }
  }
  // template for receive
  template <typename DataType>
  /**
   * @brief Wait until it receives at most capacity values from another process
   * @param values
   * @param capacity
   * @param fromProcess
   * @param tag
   */
  void receive(std::vector<DataType>& values, int capacity
      , int fromProcess = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values.data(), capacity, map(values[0]), fromProcess, tag,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw MpiError("MPI_Recv failed");
    }
  }

  // MPI_Datatype map functions
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { 
    return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { 
    return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long) { 
    return MPI_UNSIGNED_LONG_LONG; }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }
};

#endif // MPI_WRAPPER_MPI_H
