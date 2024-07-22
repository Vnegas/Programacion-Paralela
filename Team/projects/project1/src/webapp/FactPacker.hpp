// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef FACTPACKER_HPP
#define FACTPACKER_HPP

#include <vector>
#include <map>

#include "Assembler.hpp"
#include "Structs.hpp"
/**
* @brief Class that will receive all numbers and check if package is ready
* to send it to client.
*/
class FactPacker : public Assembler<reqProcessed_t, resPackage_t> {
  /// Objects of this class cannot be copied
  DISABLE_COPY(FactPacker);

 protected:
  /// map that stores packages that arent complete.
  std::map<int64_t, std::vector<reqProcessed_t>> packages;
  std::map<int64_t, int64_t> counters;
 public:
  /// Constructor
  FactPacker();
  /// Destructor
  ~FactPacker();
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the sockets, meaning, client requests
  /// @return 1 on succesful execution
  int run() override;
  /// @brief Subrutine called after a request is consumed from the
  /// goldbach calculator queue it, assembles reqProcessed_t
  /// and then it delegates the job to sendResponse, in case of an
  /// valid request process.
  /// @param data number already processed and its sums
  void consume(reqProcessed_t data) override;
};

#endif  // FACTPACKER_HPP
