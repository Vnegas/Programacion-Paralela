// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef FACTREQANALIZER_HPP
#define FACTREQANALIZER_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "Assembler.hpp"
#include "Structs.hpp"

/// @brief Class that processes request and assembles data to
/// handle each individual number by a goldbach solver
class FactReqAnalizer:public Assembler<reqRes_t, reqProcessed_t> {
 public:
  /// Counting variable that keeps track of the number of request it has
  /// received
  int64_t cont = 0;

 public:
  /// @brief constructor
  FactReqAnalizer();
  /// @brief destructor
  ~FactReqAnalizer();
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the sockets, meaning, client requests
  /// @return 1 on succesful execution
  int run() override;
  /// @brief Subrutine called after a request is consumed from the
  /// dispatcher queue it assembles reqRes_t into reqProcessed_t
  /// and then it delegates the job to goldbach calculator, in case of an
  /// valid request process
  /// @param data request to be processed
  void consume(reqRes_t data) override;
  /// Separates the numbers and save them in the vector numbers.
  /// @author Adrián Rivas <adrian.rivas@ucr.ac.cr>
  /// @author Dario Murillo <dario.murrillo@ucr.ac.cr>
  /// @author Antony Picado <antony.picado@ucr.ac.cr>
  /// @author Sebastian Venegas <sebastian.venegas@ucr.ac.cr>
  void split(std::string str, char del, std::vector<int64_t> &numbers);
  /// Replace %2C (URI comma code) with commas
  void replaceString(std::string& uri);
  /// Handle HTTP requests. Shows goldbach home page
  /// Sends the homepage as HTTP response
  void serveHomepage(reqRes_t data);
};

#endif  // FACTREQANALIZER_HPP
