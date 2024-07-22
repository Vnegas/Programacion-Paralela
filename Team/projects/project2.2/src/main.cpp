// Copyright 2023 Adrián Rivas <adrian.rivas@ucr.ac.cr>
// Copyright 2023 Dario Murillo <dario.murillo@ucr.ac.cr>
// Copyright 2023 Antony Picado <antony.picado@ucr.ac.cr>
// Copyright 2023 Sebastian Venegas <sebastian.venegas@ucr.ac.cr>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <Heatsim.hpp>


int inicio(int process_number, int data_count, int process_count) {
  int min = 0;
  if (process_number <  data_count % process_count) {
    min = process_number;
  } else {
    min = data_count % process_count;
  }
  return process_number * (data_count / process_count) + min;
}

int final(int process_number, int data_count, int process_count) {
  return inicio(process_number + 1, data_count, process_count);
}

void send(const std::string& text, int toProcess, int tag = 0) {
  if (MPI_Send(text.data(), text.length(), MPI_CHAR, toProcess
    , tag, MPI_COMM_WORLD) != MPI_SUCCESS) {
    throw std::runtime_error("No se pudo enviar el mensaje\n");
  }
}

int main(int argc, char *argv[]) {
  // MPI Inicio
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // Inicializar con un numero de proceso que no interfiera
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    // Cantidad de procesos
    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    if (process_number == 0) {
      std::vector<std::string> results;
      HeatSim obj;
      obj.analyze_arguments(argv);

      for (int i = 1; i < process_count; i++) {
        int carga_inicio = inicio(i-1, obj.info.size(), process_count-1);
        int carga_final = final(i-1, obj.info.size(), process_count-1);

        int jobs = carga_final - carga_inicio;
        if (MPI_Send(&jobs, 1, MPI_INT, i
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
          throw std::runtime_error("could not send message");
        }
        for (int j = carga_inicio; j < carga_final ; j++) {
          send(obj.info[j], i);
        }

        for (int k = carga_inicio; k < carga_final; k++) {
          std::vector<char> message(500);
          if (MPI_Recv(&message[0], 500, MPI_CHAR, i
          , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
            throw std::runtime_error("could not receive message");
          }
          results.push_back(&message[0]);
        }
      }
      std::ofstream file;
      file.open(obj.path_no_extension + ".tsv", std::ios::out);
      if (file.is_open()) {
        for (size_t i = 0; i < results.size(); i++) {
          file << results[i];
        }
        file.close();
      } else {
        throw std::runtime_error("Couldn't open file");
      }
    } else {
      int jobs = 0;
      if (MPI_Recv(&jobs, 1, MPI_INT, 0
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        throw std::runtime_error("could not receive message");
      }

      for (int i = 0; i < jobs; i++) {
        std::vector<char> message(500);
        if (MPI_Recv(&message[0], 500, MPI_CHAR, 0
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
          throw std::runtime_error("could not receive message");
        }
        std::string job = argv[1];
        HeatSim aux;
        if (argc == 3) {
          aux.num_threads = std::stoi(argv[2]);
        }
        aux.path = job.substr(0, job.find_last_of("/") + 1);
        aux.extract_data(&message[0]);
        std::string info = aux.write_report();
        send(info, 0);
      }
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
  }
  return 0;
}
