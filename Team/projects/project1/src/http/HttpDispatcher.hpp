// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef HTTPDISPATCHER_HPP
#define HTTPDISPATCHER_HPP

#include <string>
#include <vector>

#include "Dispatcher.hpp"
#include "HttpApp.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "HttpConnectionHandler.hpp"
#include "Structs.hpp"

/// @brief Class that sends request and responses to each application
class HttpDispatcher : public Dispatcher<std::string, reqRes_t> {
  DISABLE_COPY(HttpDispatcher);

 protected:
  /// Array of web applications
  std::vector<HttpApp*> applications;
  int thread_count;

 public:
  /// @brief constructor recieves vector of web applications
  explicit HttpDispatcher(std::vector<HttpApp*> applications
      , int thread_count) {
    this->applications = applications;
    this->thread_count = thread_count;
  }
  /// @brief destructor
  ~HttpDispatcher() {}
  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the reqres data type, meaning, client requests
  /// @return 1 on succesful execution
  int run() override;
  /// @brief extracts key of each request
  /// @return key or empty string
  std::string extractKey(const reqRes_t& data) const override;
  /// Sends a page for a non found resouce in this server. This method is called
  /// if none of the registered web applications handled the request.
  /// If you want to override this method, create a web app, e.g NotFoundWebApp
  /// that reacts to all URIs, and chain it as the last web app
  bool serveNotFound(reqRes_t data);
  /// @brief Subrutine called after a request is consumed from the
  /// main server queue it assembles both the httpRequest and httResponse
  /// and then it delegates the job to handle request, in case of an
  /// valid request process it until the request
  /// is closed by protocol HTTP/1.0
  /// @param data variable of Socket type that is taken from serve's queue
  void consume(reqRes_t data) override;
};

#endif  // HTTPDISPATCHER_HPP
