// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include "HttpDispatcher.hpp"

int HttpDispatcher::run() {
  int counter = 0;
  while (counter < this->thread_count) {
    this->consumeForever();
    counter++;
  }

  for (const auto& pair : this->toQueues) {
    pair.second->enqueue(this->stopCondition);
  }
  return EXIT_SUCCESS;
}

std::string HttpDispatcher::extractKey(const reqRes_t& httpData) const {
  for (size_t i = 0; i < this->applications.size(); i++) {
    if (httpData.httpRequest->getURI().rfind(this->applications[i]->ID
      , 0) == 0) {
      return this->applications[i]->ID;
    }
  }
  return "";
}

void HttpDispatcher::consume(reqRes_t data)  {
  const std::string& key = this->extractKey(data);
  if (key.compare("") != 0) {
    const auto& itr = this->toQueues.find(key);
    if ( itr == this->toQueues.end() ) {
      throw std::runtime_error("dispatcher: queue's key not found");
    }
    itr->second->enqueue(data);
  } else {
    this->serveNotFound(data);
  }
}

bool HttpDispatcher::serveNotFound(reqRes_t data) {
  // Set HTTP response metadata (headers)
  data.httpResponse->setStatusCode(404);
  data.httpResponse->setHeader("Server", "AttoServer v1.0");
  data.httpResponse->setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  data.httpResponse->body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found on this server.</p>\n"
    << "  <hr><p><a href=\"/goldbach\">Homepage</a></p>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  data.httpResponse->send();
  delete data.httpResponse;
  delete data.httpRequest;
  return true;
}
