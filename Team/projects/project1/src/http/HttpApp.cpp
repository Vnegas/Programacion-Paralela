// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <iostream>
#include "HttpApp.hpp"

void HttpApp::start() {
  // Default base class implementation does nothing
}

void HttpApp::stop() {
  // Default base class implementation does nothing
}

Queue<reqRes_t>* HttpApp::getQueue() {
  return this->queue;
}
