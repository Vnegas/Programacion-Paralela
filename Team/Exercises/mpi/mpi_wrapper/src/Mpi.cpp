// Copyright 2023 Sebastian Venegas

#include "Mpi.hpp"
#include "MpiError.hpp"

Mpi::Mpi(int &argc, char** &argv) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    MPI_Comm_size(MPI_COMM_WORLD, &this->processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &this->processNumber);
    char name[MPI_MAX_PROCESSOR_NAME];
    int nameLength = -1;
    MPI_Get_processor_name(name, &nameLength);
    this->hostname = std::string(name, nameLength);
  } else {
    throw MpiError("MPI_Init failed");
  }
}

Mpi::~Mpi() {
  MPI_Finalize();
}

int Mpi::getProcessNumber() const {
  return this->processNumber;
}

int Mpi::getProcessCount() const {
  return this->processCount;
}

std::string Mpi::getHostname() const {
  return this->hostname;
}

void Mpi::send(const std::string& text, int toProcess, int tag) {
 if (MPI_Send(text.data(), text.length(), this->map(text[0]), toProcess, tag,
      MPI_COMM_WORLD) != MPI_SUCCESS) {
    throw MpiError("MPI_Send failed");
  }
}

void Mpi::receive(std::string& text, int capacity, int fromProcess, int tag) {
  if (MPI_Recv(&text[0], capacity, MPI_CHAR, fromProcess, tag,
      MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
    throw MpiError("MPI_Recv failed");
  }
}
