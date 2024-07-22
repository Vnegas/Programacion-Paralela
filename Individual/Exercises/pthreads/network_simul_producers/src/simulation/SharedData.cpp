/// @author Sebastian Venegas Brenes
/// @copyright Sebastian Venegas Brenes 2023

#include "SharedData.hpp"

SharedData::SharedData(size_t packagesLeft)
  : packagesLeft(packagesLeft) {
}

size_t SharedData::getIndex() {
  size_t index = 0;
  mutex.lock();
  index = this->packagesLeft;
  if (index > 0) {
    --this->packagesLeft;
  }
  mutex.unlock();
  return index;
}
