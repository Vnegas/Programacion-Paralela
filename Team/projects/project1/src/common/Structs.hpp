// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <inttypes.h>
#include <vector>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

/// @brief stores request and responses
typedef struct reqRes {
  /// @brief request
  HttpRequest* httpRequest;
  /// @brief response
  HttpResponse* httpResponse;

  public:
    /// @brief default constructor
    reqRes(HttpRequest* request = nullptr, HttpResponse* response = nullptr):
    httpRequest(request), httpResponse(response) {}

    /// @brief operator overload to check stop condition
    inline bool operator==(const reqRes& other) const {
      return this->httpRequest == other.httpRequest
      && this->httpResponse == other.httpResponse;
    }
} reqRes_t;

/// @brief struc that stores data of a request that will be processed
/// by a web application
typedef struct reqProcessed {
  /// Response
  HttpResponse* response;
  /// Total numbers of the request
  int64_t totalNumbers;
  /// Position of number in the request
  int64_t pos;
  /// ID of request
  int64_t id;
  /// Number to be calculated
  int64_t number;
  /// Vector of results received from calculator
  std::vector<int64_t> results;
  /// Total goldbach sums
  int64_t sums;

 public:
  /// Constructor
  reqProcessed(HttpResponse* response = nullptr, int64_t totalNumbers = 0
    , int64_t pos = 0, int64_t id = 0, int64_t sums = 0)
    :response(response),
    totalNumbers(totalNumbers),
    pos(pos),
    id(id),
    sums(sums) {}
  /// @brief operator overload to check stop condition
  bool operator==(const reqProcessed& other) const {
    return this->response == other.response
      && this->totalNumbers == other.totalNumbers
      && this->pos == other.pos
      && this->id == other.id
      && this->sums == other.sums;
  }
  /// @brief operator overload to check if struct are differents
  bool operator!=(const reqProcessed& other) const {
    return this->response != other.response
      || this->totalNumbers != other.totalNumbers
      || this->pos != other.pos
      || this->id != other.id
      || this->sums != other.sums;
  }
} reqProcessed_t;

/// @brief struc that stores the results to be printed
typedef struct resPackage {
  /// Response
  HttpResponse* response;
  /// Results
  std::vector<reqProcessed> processed_number;

 public:
  /// Constructor
  resPackage(HttpResponse* response = nullptr
      , std::vector<reqProcessed> processed_number = {})
    :response(response), processed_number(processed_number) {}
  /// @brief operator overload to check stop condition
  bool operator==(const resPackage& other) const {
  return this->response == other.response;
  }
} resPackage_t;

#endif  // STRUCTS_HPP
