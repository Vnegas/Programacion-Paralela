//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <exception>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

// thread_shared_data_t
struct shared_data_t {
  size_t thread_count;
};

// thread_private_data_t
struct private_data_t {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
};

/**
 * @brief Prints a greet of every thread in an indeterministic way.
 * @param private_data[index], private information of every thread to work in 
 * the function. It must be initialized. Private information is the thread
 * number or rank and a pointer to the shared_data.
 */
void greet(private_data_t* private_data);
/**
 * @brief Creates and initialize private_data for each thread, and creates every
 * thread.
 * @param shared_data, public information accesible to every thread. It must be 
 * initialized. Public information is total number of threads.
 * @return An error code:
 *  0  for success.
 *  21 if could not create secondary thread
 *  22 if could not allocate n number of threads
*/
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  size_t thread_count = std::thread::hardware_concurrency();
  if (argc == 2) {
    try {
      // argv[1], "%zu", &thread_count
      std::istringstream input; 
      input.str(argv[1]);
      if (input >> thread_count) {
      } else {
        throw 11;
      }
    }
    catch (int exception) {
      std::cerr << "Error: invalid thread count" << std::endl;
      return exception;
    }
  }

  try {
    shared_data_t* shared_data = (shared_data_t*)calloc(1
      , sizeof(shared_data_t));
    if (shared_data) {
      shared_data->thread_count = thread_count;

      std::chrono::high_resolution_clock::time_point start_time 
        = std::chrono::high_resolution_clock::now();

      error = create_threads(shared_data);

      std::chrono::high_resolution_clock::time_point finish_time 
        = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> elapsed_time = std::chrono::duration_cast
        <std::chrono::duration<double>>(finish_time - start_time);

      std::cout << "Execution time: " << elapsed_time.count() << "s" << 
        std::endl;

      free(shared_data);
    } else {
      throw 12;
    }
  }
  catch (int exception) {
    std::cerr << "Error: could not allocate shared data" << std::endl;
    return exception;
  }
  return error;
}  // end procedure

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  std::vector<std::thread> threads;

  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (private_data) {
    for (size_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {

      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;

      threads.emplace_back(std::thread(greet, &private_data));
    }

    // print "Hello from main thread"
    std::cout << "Hello from main thread" << std::endl;

    for (std::thread & th : threads) {
      if (th.joinable()) {
        th.join();
      }
    }

    free(private_data); // ****
  } else {
    std::cerr << "Error: could not allocate" << shared_data->thread_count << 
      "threads" << std::endl;
    error = 22;
  }

  return error;
}

// procedure greet:
void greet(private_data_t* private_data) {
  shared_data_t* shared_data = private_data->shared_data;

  // print "Hello from secondary thread"
  std::cout << "Hello from secondary thread" << private_data->thread_number 
    << " of " << shared_data->thread_count << std::endl;
}  // end procedure
