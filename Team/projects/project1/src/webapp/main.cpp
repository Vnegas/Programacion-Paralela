// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>
// concurrent web server's

#include <csignal>
#include <iostream>

#include "HttpServer.hpp"
#include "GoldbachWebApp.hpp"
#include "FactWebApp.hpp"
#include "HttpConnectionHandler.hpp"

/// @brief  Handles signal to kill or interrupt the program
/// @param signalNum
void signalHandler(int signalNum) {
  (void)signalNum;
  std::cout << "\nInterrupt signal from " << getpid() << std::endl;
  HttpServer::getInstance().stop();
}

/// Start the web server
int main(int argc, char* argv[]) {
  // Create a goldbach web application.
  GoldbachWebApp goldbachWebApp("/goldbach");
  // Create a factorization web application.
  FactWebApp fact("/fact");
  // Register the web application(s) with the web server
  HttpServer::getInstance().chainWebApp(&goldbachWebApp);
  HttpServer::getInstance().chainWebApp(&fact);
  // signal to kill or interrupt program
  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
  // Run the web server
  HttpServer::getInstance().run(argc, argv);
  return 0;
}
