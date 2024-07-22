// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <iostream>

#include "GoldReqAnalizer.hpp"

GoldReqAnalizer::GoldReqAnalizer() {}

GoldReqAnalizer::~GoldReqAnalizer() {}

int GoldReqAnalizer::run() {
  this->consumeForever();
  int64_t calculator_count = std::thread::hardware_concurrency();
  for (int index = 0; index < calculator_count ; ++index) {
    this->produce(reqProcessed());
  }
  return EXIT_SUCCESS;
}

void GoldReqAnalizer::consume(reqRes_t data) {
  std::vector<int64_t> numbers;
  std::smatch matches;
  // cambiar las %C2 por , en el URI
  std::string uri;
  uri = data.httpRequest->getURI();
  this->replaceString(uri);
  std::regex inQuery("^/goldbach(/|\\?numbers=)([-?\\d+,?]*)$");
  std::regex_search(uri, matches, inQuery);
  if (matches.length() >= 3) {
    char del = ',';
    this->split(matches[2], del, numbers);
    for (size_t i = 0; i < numbers.size(); i++) {
      reqProcessed datos(data.httpResponse, numbers.size(), i, this->cont);
      datos.number = numbers[i];
      this->produce(datos);
    }
    delete data.httpRequest;
    ++this->cont;
  } else {
    serveHomepage(data);
  }
}

void GoldReqAnalizer::split(std::string str, char del
    , std::vector<int64_t> &numbers) {
  std::string number;
  for (size_t number_char = 0; number_char < str.size(); ++number_char) {
    // if not found the delimitator, next number is concatenated to number.
    if (str[number_char] != del) {
      number += str[number_char];
    } else {  // the number is push towards numbers and clean the number.
      numbers.push_back(std::stoi(number));
      number = "";
    }
  }
  numbers.push_back(std::stoi(number));
}

void GoldReqAnalizer::replaceString(std::string& uri) {
  for (size_t i = 0; i < uri.size(); ++i) {
    if (uri[i] == '%') {
      uri.replace(i, 3, ",");
    }
  }
}

void GoldReqAnalizer::serveHomepage(reqRes_t data) {
  // Set HTTP response metadata (headers)
  data.httpResponse->setHeader("Server", "AttoServer v1.1");
  data.httpResponse->setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Goldbach";
  data.httpResponse->body() << "<!DOCTYPE html>\n"
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
  data.httpResponse->send();
  delete data.httpResponse;
  delete data.httpRequest;
}
