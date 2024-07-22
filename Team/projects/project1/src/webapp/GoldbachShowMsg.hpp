// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef GOLDBACHSHOWMSG_HPP
#define GOLDBACHSHOWMSG_HPP

#include <inttypes.h>
#include <thread>
#include <string>
#include <vector>

#include "Consumer.hpp"

#include "HttpResponse.hpp"
#include "Structs.hpp"

/**
* @brief A web application that shows the results to the client
*/
class GoldbachShowMsg : public Consumer<resPackage_t> {
 public:
  /// @brief constructor
  GoldbachShowMsg();
  /// @brief destructor
  ~GoldbachShowMsg();
  /// @brief Subrutine called after a request is consumed from the
  /// packer queue it consumes resPackage_t and shows results
  /// @param data request to be processed
  void consume(resPackage_t data) override;
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the client requests
  /// @return 1 on succesful execution
  int run() override;

 private:
  /// Shows numbers and results calculated
  void serveGoldbachResponse(resPackage_t& data);
  /// Shows error message in HTML if number was invalid
  void printError(resPackage_t& data);
};

#endif  // GOLDBACHSHOWMSG_HPP
