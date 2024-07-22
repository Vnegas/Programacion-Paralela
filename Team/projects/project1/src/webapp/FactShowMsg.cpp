// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include "FactShowMsg.hpp"

FactShowMsg::FactShowMsg() {}

FactShowMsg::~FactShowMsg() {}

int FactShowMsg::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

void FactShowMsg::consume(resPackage_t data) {
  this->servePrimFactResponse(data);
}

void FactShowMsg::printError(resPackage_t& data) {
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

void FactShowMsg::servePrimFactResponse(resPackage_t& data) {
  std::string title = "Prime Factorization";

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
      if ((data.processed_number[data_number].number == 1)
          || (data.processed_number[data_number].number == 0)) {
        data.response->body() << "  <p>" << "NA\n";
      } else if (data.processed_number[data_number].number < 0) {
        data.response->body() << "  <p>" << "invalid number\n";
      } else {
        for (size_t index = 0; index < data.processed_number[data_number]
          .results.size(); ++index) {
          data.response->body() << " "
            << data.processed_number[data_number].results[index];
          if (index + 1 != data.processed_number[data_number].results.size()) {
            if (data.processed_number[data_number].results[index]
              == data.processed_number[data_number].results[index + 1]) {
              int64_t exponent = 1;
              while (data.processed_number[data_number].results[index]
                  == data.processed_number[data_number].results[index + 1]) {
                index++;
                exponent++;
                if (index + 1 >=
                data.processed_number[data_number].results.size()) { break;}
              }
              data.response->body() << "^" << exponent;
            }
          }
        }
      }
    }
    data.response->body() << "  <hr><p><a href=\"/fact\">Back</a></p>\n"
    << "</html>\n";

  data.response->send();
  delete data.response;
}
