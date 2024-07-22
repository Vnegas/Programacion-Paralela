//  Copyright 2023 Sebastian Venegas

#ifndef SIMULATION_H
#define SIMULATION_H

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "consumer.h"
#include "producer.h"

typedef struct simulation simulation_t;  // opaque record

/**
 * @brief initialize the simulation and its attributes
 * @return object of the simulation
 */
simulation_t* simulation_create();
/**
 * @brief Creates private data objects
 * @param simulation 
 * @return object of private_data
 */
private_data_t* create_private_data(simulation_t* simulation);
/**
 * @brief starts the simulation, main function
 * @param simulation
 * @param argc
 * @param argv
 * @return int error code
 */
int simulation_run(simulation_t* simulation, int argc, char* argv[]);
/**
 * @brief destroys and frees simulation and memory allocated
 * @param simulation 
 */
void simulation_destroy(simulation_t* simulation);
/**
 * @brief destroys and frees private_data and memory allocated
 * @param priv_data
 * @param simulation
 */
void private_data_destroy(private_data_t* priv_data, simulation_t* simulation);
/**
 * @brief adds new element to the simulation
 * @param simulation
 * @param file_path
 * @return int error code
 */
int append(simulation_t* simulation, char* file_path);
/**
 * @brief copies into a temp dir all zip files located in paths stored in data
 * @param priv_data
 * @param file_num
 * @param thread_num
*/
int copy_files(private_data_t* priv_data, size_t file_num, size_t thread_num);
/**
 * @brief deletes an entire directory and the files inside it
 * @param path
*/
int remove_directory(const char* path);

#endif  // SIMULATION_H
