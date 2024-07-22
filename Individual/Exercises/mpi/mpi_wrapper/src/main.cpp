// Copyright 2023 Sebastian Venegas

#include "Mpi.hpp"
#include "MpiError.hpp"

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();
    const int previous = (count + rank - 1) % count;
    const int next = (rank + 1) % count;

    bool can_print = true;
    if (rank > 0) {
      mpi.receive(can_print, previous);
    }

    std::cout << "Hello from main thread of process " << rank + 1 << " of "
      << count << " on " << mpi.getHostname() << std::endl;

    mpi.send(can_print, next);
  } catch (const MpiError& mpierror) {
    std::cerr << "error: " << mpierror.what() << std::endl;
  }
}
