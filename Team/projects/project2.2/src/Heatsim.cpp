// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>

#include <float.h>
#include <math.h>
#include <sys/stat.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "Heatsim.hpp"

void HeatSim::analyze_arguments(char *argv[]) {
  std::string job = argv[1];
  this->path = job.substr(0, job.find_last_of("/") + 1);
  std::string report = this->path + "report";
  const char* aux = report.c_str();
  mkdir(aux, 0777);
  this->path_no_extension = job.substr(0, job.find_last_of("."));
  std::string input;
  std::ifstream file;
  file.open(job, std::ifstream::in);
  if (file.is_open()) {
    while (std::getline(file, input)) {
      this->info.push_back(input);
    }
    file.close();
    this->jobs.resize(this->info.size());
  } else {
    throw std::runtime_error("Couldn't open file");
  }
}

void HeatSim::extract_data(std::string txt) {
  this->jobs.resize(1);
  std::string aux = "";
  std::vector<std::string> tmp;
  for (size_t j = 0; j < txt.size(); j++) {
    if (txt[j] != ' ') {
      aux = aux + txt[j];
      if (j == txt.size() - 1) {
        tmp.push_back(aux);
      }
    } else {
      if (aux != "") {
        tmp.push_back(aux);
      }
      aux = "";
    }
  }
  for (size_t k = 0; k < tmp.size(); k++) {
    switch (k) {
      case 0:
        this->jobs[0].plate_name = tmp[k];
        break;
      case 1:
        this->jobs[0].delta_time = std::stod(tmp[k]);
        break;
      case 2:
        this->jobs[0].diffusion = std::stod(tmp[k]);
        break;
      case 3:
        this->jobs[0].dimensions = std::stod(tmp[k]);
        break;
      case 4:
        this->jobs[0].epsilon = std::stod(tmp[k]);
        break;
      default:
        break;
    }
  }
  fill_matrix();
}

void HeatSim::fill_matrix() {
  // itera sobre todas las placas
  for (size_t i = 0; i < this->jobs.size(); i++) {
    std::string tmp_path;
    tmp_path = this->path + this->jobs[i].plate_name;
    std::ifstream file(tmp_path, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Couldn't open file");
    }
    size_t rows = 0;
    size_t cols = 0;
    file.read(reinterpret_cast<char*>(&rows), 8);
    file.read(reinterpret_cast<char*>(&cols), 8);
    this->jobs[i].rows = rows;
    this->jobs[i].cols = cols;

    double number = 0.0;
    this->jobs[i].current_plate =
    std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    this->jobs[i].temp_plate =
    std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        number = 0.0;
        file.read(reinterpret_cast<char*>(&number), 8);
        this->jobs[i].current_plate[row][col] = number;
        this->jobs[i].temp_plate[row][col] = number;
      }
    }
    file.close();
    this->transfer_heat(i);
  }
}

void HeatSim::transfer_heat(int index) {
  while (this->state_change(index) == false) {}
  write_binary_matrix();
}

bool HeatSim::state_change(int index) {
  double max_change = DBL_MIN;
  int64_t threads = this->num_threads;
  size_t rows = this->jobs[index].rows - 1;
  size_t cols = this->jobs[index].cols - 1;
  #pragma omp parallel for num_threads(threads) schedule(static) \
    default(none) shared(rows, cols, index) \
    collapse(2) reduction(max:max_change)
  for (size_t row = 1; row < rows; ++row) {
    for (size_t col = 1; col < cols; ++col) {
      double change_cell = 0.0;
      // #pragma omp critical
      change_cell = this->calculate_temp(index, row, col);
      if (change_cell > max_change) {
        max_change = change_cell;
      }
    }
  }
  this->jobs[index].k++;
  this->jobs[index].time_elapsed =
    this->jobs[index].delta_time * this->jobs[index].k;
  std::swap(this->jobs[index].current_plate, this->jobs[index].temp_plate);
  if (max_change > this->jobs[index].epsilon) {
    return false;
  } else {
    return true;
  }
}

double HeatSim::calculate_temp(int index, int row, int col) {
  this->jobs[index].temp_plate[row][col] =
  this->jobs[index].current_plate[row][col]+
  (((this->jobs[index].delta_time) * (this->jobs[index].diffusion)) /
  (pow(this->jobs[index].dimensions, 2))) *
  (this->jobs[index].current_plate[row - 1][col]
  + this->jobs[index].current_plate[row][col + 1]
  + this->jobs[index].current_plate[row][col - 1]
  + this->jobs[index].current_plate[row + 1][col]
  - 4 * this->jobs[index].current_plate[row][col]);

  return fabs(this->jobs[index].temp_plate[row][col]
    - this->jobs[index].current_plate[row][col]);
}

std::string HeatSim::write_report() {
  std::vector<std::string> results;
  std::stringstream buffer;
  buffer
    << this->jobs[0].plate_name << "\t"
    << this->jobs[0].delta_time << "\t"
    << this->jobs[0].diffusion << "\t"
    << this->jobs[0].dimensions << "\t"
    << this->jobs[0].epsilon << "\t"
    << this->jobs[0].k << "\t"
    << format_time(this->jobs[0].time_elapsed) << "\n";
  std::string aux = buffer.str();
  return aux;
}

void HeatSim::write_binary_matrix() {
  for (size_t i = 0; i < this->jobs.size(); i++) {
    std::string plate_no_extension
    = this->jobs[i].plate_name.substr
    (0, this->jobs[i].plate_name.find_last_of("."));

    std::stringstream buffer;
    buffer << this->path << "report/" << plate_no_extension
    << "-" << this->jobs[i].k << ".bin";
    std::string output =  buffer.str();
    std::ofstream file;
    file.open(output, std::ios::out | std::ios::binary);
    if (file.is_open()) {
      size_t rows = this->jobs[i].current_plate.size();
      size_t cols = this->jobs[i].current_plate[0].size();
      file.write(reinterpret_cast<char*>(&rows), 8);
      file.write(reinterpret_cast<char*>(&cols), 8);
      // Writes it
      for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
          file.write
          (reinterpret_cast<char*>(&this->jobs[i].current_plate[row][col]),
          sizeof(double));
        }
      }
      file.close();
    } else {
      throw std::runtime_error("Couldn't open file");
    }
  }
}

std::string HeatSim::format_time(const time_t seconds) {
  char text[48];
  const std::tm& gmt = * std::gmtime(&seconds);
  snprintf(text, sizeof text, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt.tm_year
    - 70, gmt.tm_mon, gmt.tm_mday - 1, gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
  return text;
}
