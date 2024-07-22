// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef FACTCALCULATOR_HPP
#define FACTCALCULATOR_HPP

#include <vector>
#include "Assembler.hpp"
#include "Structs.hpp"


/// @brief  Class that calculates prime factorization of numbers
class FactCalculator: public Assembler<reqProcessed, reqProcessed>{
 public:
  /// Constructor
  FactCalculator();
  /// Destructor
  ~FactCalculator();
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the reqRest_t
  /// @return 1 on succesful execution
  int run() override;
  /// @brief Subrutine called after a request is consumed from the
  /// main server queue it assembles both the httpRequest and httResponse
  /// and then it delegates the job to handle request, in case of an
  /// valid request process it until the request
  /// is closed by protocol HTTP/1.0
  /// @param data variable of Socket type that is taken from serve's queue
  void consume(reqProcessed_t data) override;
  /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
};

#endif  // FACTCALCULATOR_HPP
