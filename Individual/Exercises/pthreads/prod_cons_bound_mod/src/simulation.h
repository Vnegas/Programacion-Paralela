//  Copyright 2023 Sebastian Venegas

#ifndef SIMULATION_H
#define SIMULATION_H

int create_threads(shared_data_t* shared_data);
void simulation_destroy(shared_data_t* shared_data);
int simulation_run(shared_data_t* shared_data, int argc, char* argv[]);

#endif  // SIMULATION_H
