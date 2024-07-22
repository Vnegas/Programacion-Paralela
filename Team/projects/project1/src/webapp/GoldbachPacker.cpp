// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include "GoldbachPacker.hpp"

GoldbachPacker::GoldbachPacker() {}

GoldbachPacker::~GoldbachPacker() {}

int GoldbachPacker::run() {
  size_t counter = 0;
  while (counter < std::thread::hardware_concurrency()) {
    this->consumeForever();
    counter++;
  }
  this->produce(resPackage());
  return EXIT_SUCCESS;
}

void GoldbachPacker::consume(reqProcessed_t data) {
  std::map<int64_t, std::vector<reqProcessed_t>>::iterator it =
    this->packages.find(data.id);
  if (it != this->packages.end()) {
    this->packages[data.id][data.pos] = data;
    ++this->counters[data.id];
    if (this->counters[data.id] == data.totalNumbers) {
      resPackage_t resData(data.response
        , this->packages[data.id]);
      produce(resData);
    }
  } else {
    // Vector that packs data received.
    std::vector<reqProcessed_t> package;
    package.resize(data.totalNumbers);
    package.insert(package.begin() + data.pos, data);
    this->packages.emplace(data.id, package);
    this->counters.emplace(data.id, 1);
    if (this->counters[data.id] == data.totalNumbers) {
      resPackage_t resData(data.response
        , this->packages[data.id]);
      produce(resData);
    }
  }
}
