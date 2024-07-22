//  Copyright 2023 Sebastian Venegas

#ifndef PRODUCER_H
#define PRODUCER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

/**
 * @brief starts process to find passwords. It calculates every variation and
 * produces it
 * @param data
*/
void* produce(void* data);

#endif  // PRODUCER_H
