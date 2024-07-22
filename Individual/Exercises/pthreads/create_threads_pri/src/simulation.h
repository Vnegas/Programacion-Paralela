//  Copyright 2023 Sebastian Venegas

#ifndef SIMULATION_H
#define SIMULATION_H

typedef struct simulation simulation_t;  // opaque record

simulation_t* simulation_create();
int simulation_run(simulation_t* simulation, int argc, char* argv[]);
void simulation_destroy(simulation_t* simulation);

#endif  // SIMULATION_H
