//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <stdlib.h>

#include "common.h"

useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
