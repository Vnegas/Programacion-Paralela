// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cassert>

/// @brief Llena una matriz segun los valores de un archivo binario
/// @param filename Dirreccion del archivo
/// @return Matriz llena con los valores
std::vector<std::vector<double>> fill_matrix(std::string filename);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (argc != 4) {
    std::cerr << "Must provide valid parameters" << std::endl;
    error = EXIT_FAILURE;
    return error;
  }
  std::string primer_dirr = argv[1];
  std::string segunda_dirr = argv[2];
  double epsilon = std::stod(argv[3]);
  std::vector<std::vector<double>> primer_matriz = fill_matrix(primer_dirr);
  std::vector<std::vector<double>> segunda_matriz = fill_matrix(segunda_dirr);
  // Revisa si tienen las mismas dimensiones
  if (primer_matriz.size() == segunda_matriz.size() 
  && primer_matriz[0].size() == segunda_matriz[0].size() ) {
    for (size_t row = 0; row < primer_matriz.size(); ++row) {
      for (size_t col = 0; col < primer_matriz[0].size(); ++col) {
        double valor1 = primer_matriz[row][col];
        double valor2 = segunda_matriz[row][col];
        if (fabs(valor1 - valor2) > epsilon) {
          std::cout << 
          "Value difference in row " << row << " col " << col <<
          " is higher than epsylon provided" << std::endl;
          error = EXIT_FAILURE;
          return error;
        }
      }
    }
  } else {
    std::cerr << "Matrix have differente dimensions" << std::endl;
    error = EXIT_FAILURE;
    return error;
  }
  std::cout << "Everything okay" << std::endl;
  return error;
}

std::vector<std::vector<double>> fill_matrix(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Couldn't open file");
  }
  size_t rows = 0;
  size_t cols = 0;
  file.read((char*)&rows, 8);
  file.read((char*)&cols, 8);

  double number = 0.0;
  std::vector<std::vector<double>> matriz = 
  std::vector<std::vector<double>>(rows, std::vector<double>(cols));

  for (size_t row = 0; row < rows; row++) {
    for (size_t col = 0; col < cols; col++) {
      number = 0.0;
      file.read((char*)&number, 8);
      matriz[row][col] = number;
    }
  }
  file.close();
  return matriz;
}
