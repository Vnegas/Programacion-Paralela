// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <stdlib.h>
#include <iostream>

#include "GoldbachCalculator.hpp"
#include "GoldbachModel.hpp"

GoldbachCalculator::GoldbachCalculator() {
}

GoldbachCalculator::~GoldbachCalculator() {
}

int GoldbachCalculator::run() {
  this->consumeForever();
  this->produce(reqProcessed());
  return EXIT_SUCCESS;
}

void GoldbachCalculator::consume(reqProcessed_t data) {
  if (data != reqProcessed()) {
    GoldbachModel* model = new GoldbachModel();
    model->calculateSums(data.number, data.results, data.sums);
    this->produce(data);
    delete model;
  }
}
