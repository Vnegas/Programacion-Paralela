// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <math.h>

#include <stdlib.h>
#include <iostream>

#include "FactCalculator.hpp"
#include "FactModel.hpp"

FactCalculator::FactCalculator() {}

FactCalculator::~FactCalculator() {}


int FactCalculator::run() {
  this->consumeForever();
  this->produce(reqProcessed());
  return EXIT_SUCCESS;
}

void FactCalculator::consume(reqProcessed_t data) {
  if (data != reqProcessed()) {
    FactModel* model = new FactModel();
    model->prime_fact(data.number, data.results);
    this->produce(data);
    delete model;
  }
}
