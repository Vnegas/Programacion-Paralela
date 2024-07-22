// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#ifndef HTTPSCONNECTIONHANDLER_HPP
#define HTTPSCONNECTIONHANDLER_HPP

#include <vector>
#include <Assembler.hpp>
#include <Socket.hpp>
#include <HttpRequest.hpp>
#include <HttpServer.hpp>
#include <HttpApp.hpp>
#include "Structs.hpp"

/// Class used to manage the connections of the server
class HttpConnectionHandler : public Assembler<Socket, reqRes_t> {
  DISABLE_COPY(HttpConnectionHandler);

 public:
  /// Class constructor, initialize vector of web apps
  HttpConnectionHandler() {}
  /// Class destructor
  ~HttpConnectionHandler() {}

  /// @brief main subrutine that will be done by the threads responsible
  /// of handling the sockets, meaning, client requests
  /// @return 1 on succesful execution
  int run() override;
  /// @brief Subrutine called after a request is consumed from the
  /// main server queue it assembles both the httpRequest and httResponse
  /// and then it delegates the job to handle request, in case of an
  /// valid request process it until the request
  /// is closed by protocol HTTP/1.0
  /// @param data variable of Socket type that is taken from serve's queue
  void consume(Socket data) override;
  /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
  virtual bool handleHttpRequest(HttpRequest*httpRequest,
    HttpResponse* httpResponse);
};

#endif  // HTTPSCONNECTIONHANDLER_HPP
