/// @author Sebastian Venegas Brenes
/// @copyright Sebastian Venegas Brenes 2023

#ifndef SHAREDDATA_HPP
#define SHAREDDATA_HPP

#include <cstddef>
#include <cstdlib>
#include <mutex>

#include "common.hpp"

class SharedData {
 protected:
  std::mutex mutex;
  size_t packagesLeft = 0;
 public:
  // Constructor
  explicit SharedData(size_t packagesLeft);
  // Destructor
  ~SharedData() {
  }
  size_t getIndex();
};

#endif  // SHAREDDATA_HPP
