// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <stdio.h>
#include <string>

#include "HttpConnectionHandler.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "HttpApp.hpp"

int HttpConnectionHandler::run() {
  this->consumeForever();
  this->produce(reqRes());
  return EXIT_SUCCESS;
}

void HttpConnectionHandler::consume(Socket data) {
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest* httpRequest = new HttpRequest(data);

    // If the request is not valid or an error happened
    if (!httpRequest->parse()) {
      delete httpRequest;
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse* httpResponse = new HttpResponse(data);

    // Give subclass a chance to respond the HTTP request
    const bool handled = this->handleHttpRequest(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0

    // "HTTP/1.1 para navegador, preferiblemente Mozilla"
    // "HTTP/1.0 para las pruebas httperf"
    if (!handled || httpRequest->getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      delete httpRequest;
      delete httpResponse;
      data.close();
      break;
    }
  }
}

bool HttpConnectionHandler::handleHttpRequest(HttpRequest* httpRequest,
    HttpResponse* httpResponse) {
  /// struct for requests and responses.
  reqRes_t httpData;
  httpData.httpRequest = httpRequest;
  httpData.httpResponse = httpResponse;

  // Print IP and port from client
  const NetworkAddress& address = httpRequest->getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  Log::append(Log::INFO, "request", httpRequest->getMethod()
    + ' ' + httpRequest->getURI()
    + ' ' + httpRequest->getHttpVersion());

  this->produce(httpData);
  return true;
}

