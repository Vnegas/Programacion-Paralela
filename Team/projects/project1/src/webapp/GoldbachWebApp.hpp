// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <inttypes.h>
#include <thread>
#include <string>
#include <vector>

#include "HttpApp.hpp"
#include "HttpDispatcher.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include "GoldbachCalculator.hpp"
#include "GoldbachPacker.hpp"
#include "GoldbachShowMsg.hpp"
#include "GoldReqAnalizer.hpp"
#include "Queue.hpp"

/**
* @brief A web application that calculates prime factors
*/
class GoldbachWebApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(GoldbachWebApp);

 public:
  /// Constructor
  explicit GoldbachWebApp(std::string ID);
  /// Destructor
  ~GoldbachWebApp();
  /// Called by the web server when the web server is started
  void start() override;
  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  void stop() override;
  /// Thread team of goldbach calculators
  int64_t calculator_count = std::thread::hardware_concurrency();
  std::vector<GoldbachCalculator*> calculators;
  /// Object that will take the request of the dispatcher and
  /// will enqueue in the goldbach application
  GoldReqAnalizer* goldbachAnalizer;
  /// Queue of consume of the Goldbach Analizer.
  Queue<reqProcessed_t>* appQueue;
  /// Objects that packs all of results from numbers in request
  GoldbachPacker* goldbachPacker;
  /// Objects that shows all of results from numbers in request
  GoldbachShowMsg* goldbachMsg;

 protected:
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// Sends the homepage as HTTP response
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Handle a HTTP request that starts with "/fact"
  /// @return true if the factorization was handled, false if it must be
  /// handled by another application
  // bool serveGoldbach(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// handle request does nothing, declared because is a pure virtual method
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;
};

#endif  // FACTWEBAPP_HPP
