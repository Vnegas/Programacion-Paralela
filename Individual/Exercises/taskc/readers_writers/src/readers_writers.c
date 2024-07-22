// Copyright 2023 Sebastian Venegas

#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  size_t reader_count = 0;
  size_t writer_count = 0;
  char letter = '\0';
  #pragma omp parallel shared(reader_count, writer_count)
  {
    while (scanf("%c", &letter) == 1) {
      if (toupper(letter) == 'R') {
        #pragma omp critical
        ++reader_count;
        printf("Reader got %zu\n", reader_count);
        read();
      } else if (toupper(letter) == 'W') {
        #pragma omp critical
        ++writer_count;
        printf("Writer increased to %zu\n", writer_count);
      }
    }
  }
  return 0;
}
