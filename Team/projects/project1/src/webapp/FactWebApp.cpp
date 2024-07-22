// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>

#include "FactWebApp.hpp"

FactWebApp::FactWebApp(std::string ID):HttpApp(ID) {
}

FactWebApp::~FactWebApp() {
}

void FactWebApp::start() {
  try {
    this->appQueue = new Queue<reqProcessed_t>();
    this->factAnalizer = new FactReqAnalizer();
    this->factPacker = new FactPacker();
    this->factMsg = new FactShowMsg();
    this->factAnalizer->setConsumingQueue(this->getQueue());
    this->factAnalizer->setProducingQueue(this->appQueue);
    this->factPacker->createOwnQueue();
    this->factMsg->createOwnQueue();
    this->calculators.resize(this->calculator_count);
    for (int64_t index = 0; index < this->calculator_count; ++index) {
      this->calculators[index] = new FactCalculator();
      this->calculators[index]->setConsumingQueue
        (this->factAnalizer->getProducingQueue());
      this->calculators[index]->setProducingQueue
        (this->factPacker->getConsumingQueue());
    }
    this->factPacker->setProducingQueue
      (this->factMsg->getConsumingQueue());

    this->factAnalizer->startThread();
    for (int64_t index = 0; index < this->calculator_count; ++index) {
      this->calculators[index]->startThread();
    }
    this->factPacker->startThread();
    this->factMsg->startThread();
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    stop();
  }
}

void FactWebApp::stop() {
  this->factAnalizer->waitToFinish();
  for (int64_t index = 0; index < this->calculator_count; ++index) {
    this->calculators[index]->waitToFinish();
  }
  this->factPacker->waitToFinish();
  this->factMsg->waitToFinish();

  delete this->factMsg;
  delete this->factPacker;
  for (int64_t index = 0; index < this->calculator_count; ++index) {
    delete this->calculators[index];
  }
  delete this->factAnalizer;
  delete this->appQueue;
}

bool FactWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  (void) httpRequest;
  (void) httpResponse;
  return true;
}

bool FactWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

    // Set HTTP response metadata (headers)
    httpResponse.setHeader("Server", "AttoServer v1.1");
    httpResponse.setHeader("Content-type", "text/html; charset=ascii");

    // Build the body of the response
    std::string title = "Goldbach";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/goldbach\">\n"
    << "    <label for=\"numbers\">Numbers</label>\n"
    << "    <input type=\"text\" name=\"numbers\" required/>\n"
    << "    <button type=\"submit\">Goldbach sums</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  httpResponse.send();
  return true;
}
