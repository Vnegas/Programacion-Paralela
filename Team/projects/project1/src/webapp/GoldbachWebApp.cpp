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

#include "GoldbachWebApp.hpp"

GoldbachWebApp::GoldbachWebApp(std::string ID):HttpApp(ID) {
}

GoldbachWebApp::~GoldbachWebApp() {
}

void GoldbachWebApp::start() {
  try {
    this->appQueue = new Queue<reqProcessed_t>();
    this->goldbachAnalizer = new GoldReqAnalizer();
    this->goldbachPacker = new GoldbachPacker();
    this->goldbachMsg = new GoldbachShowMsg();
    this->goldbachAnalizer->setConsumingQueue(this->getQueue());
    this->goldbachAnalizer->setProducingQueue(this->appQueue);
    this->goldbachPacker->createOwnQueue();
    this->goldbachMsg->createOwnQueue();
    this->calculators.resize(this->calculator_count);
    for (int64_t index = 0; index < this->calculator_count; ++index) {
      this->calculators[index] = new GoldbachCalculator();
      this->calculators[index]->setConsumingQueue
        (this->goldbachAnalizer->getProducingQueue());
      this->calculators[index]->setProducingQueue
        (this->goldbachPacker->getConsumingQueue());
    }
    this->goldbachPacker->setProducingQueue
      (this->goldbachMsg->getConsumingQueue());

    this->goldbachAnalizer->startThread();
    for (int64_t index = 0; index < this->calculator_count; ++index) {
      this->calculators[index]->startThread();
    }
    this->goldbachPacker->startThread();
    this->goldbachMsg->startThread();
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    stop();
  }
}

void GoldbachWebApp::stop() {
  this->goldbachAnalizer->waitToFinish();
  for (int64_t index = 0; index < this->calculator_count; ++index) {
    this->calculators[index]->waitToFinish();
  }
  this->goldbachPacker->waitToFinish();
  this->goldbachMsg->waitToFinish();

  delete this->goldbachMsg;
  delete this->goldbachPacker;
  for (int64_t index = 0; index < this->calculator_count; ++index) {
    delete this->calculators[index];
  }
  delete this->goldbachAnalizer;
  delete this->appQueue;
}

bool GoldbachWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  (void) httpRequest;
  (void) httpResponse;
  return true;
}

bool GoldbachWebApp::serveHomepage(HttpRequest& httpRequest
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
