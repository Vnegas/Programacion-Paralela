// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <iostream>

#include "GoldbachShowMsg.hpp"

GoldbachShowMsg::GoldbachShowMsg() {}

GoldbachShowMsg::~GoldbachShowMsg() {}

int GoldbachShowMsg::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

void GoldbachShowMsg::consume(resPackage_t data) {
  this->serveGoldbachResponse(data);
}

void GoldbachShowMsg::printError(resPackage_t& data) {
  std::string title = "Invalid request";
    data.response->body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1>\n"
      << "  <p>Invalid request for goldbach sum</p>\n"
      << "  <hr><p><a href=\"/goldbach\">Back</a></p>\n"
      << "</html>\n";
  delete data.response;
}

void GoldbachShowMsg::serveGoldbachResponse(resPackage_t& data) {
    std::string title = "Goldbach Sums";

  data.response->body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n";

  for (size_t data_number = 0; data_number < data.processed_number.size() -1
    ; ++data_number) {
    data.response->body() << "  <h2>"
      << data.processed_number[data_number].number << "</h2>\n";
    if (data.processed_number[data_number].number >= -5
      && data.processed_number[data_number].number <= 5) {
      data.response->body() << "  <p>" << "NA\n";
    }
    if (data.processed_number[data_number].number > 5) {
      data.processed_number[data_number].response->body()
        << "  <p>" << "Total Sums: "
          << data.processed_number[data_number].sums << "  <p>";
    }
    if (data.processed_number[data_number].number < -5) {
      data.response->body() << "  <p>" << "Total Sums: "
        << data.processed_number[data_number].sums << "  <p>";
      size_t pos = 0;
      for (size_t index = 0; index < data.processed_number[data_number]
          .results.size(); ++index) {
        if (data.processed_number[data_number].number % 2 == 0) {
          data.response->body() << " " << data.processed_number[data_number]
            .results[index];
          if (pos == 1) {
            data.response->body() << "<p>";
            pos = 0;
          } else {
            data.response->body() << " + ";
            ++pos;
          }
        } else {
          data.response->body() << " " << data.processed_number[data_number]
            .results[index];
          if (pos == 2) {
            data.response->body() << "<p>";
            pos = 0;
          } else {
            data.response->body() << " + ";
            ++pos;
          }
        }
      }
    }
  }
  data.response->body() << "  <hr><p><a href=\"/goldbach\">Back</a></p>\n"
    << "</html>\n";

  data.response->send();
  delete data.response;
}
