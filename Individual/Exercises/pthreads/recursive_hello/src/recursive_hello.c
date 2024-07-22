//  Copyright 2023 Sebastian Venegas

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief prints text from a secondary thread, it creates other threads using 
 * recursivity.
 * @param var, a number of threads to create.
 * @return NULL
 */
void* greet(void* data);


int main(void) {
  size_t var = 2;
  // crear hilo(greet)
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, &var);
  // if hilo creado correctamente
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
    // pthread_join(thread, var, NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // fin

// greet:
void* greet(void* data) {
  size_t* number = (size_t*) data;
  if (*number == 0) {
    printf("Goodbye from secondary thread %zu\n", (size_t)*number);
  } else if (*number > 0) {
    printf("Hello from secondary thread %zu\n", (size_t)*number);
    *number = *number - 1;
    pthread_t thread;
    int error = pthread_create(&thread, /*attr*/ NULL, greet, number);
    // if hilo creado correctamente
    if (error == EXIT_SUCCESS) {
      pthread_join(thread, /*value_ptr*/ NULL);
    } else {
      fprintf(stderr, "Error: could not create secondary thread\n");
    }
  }
  return NULL;
}  // fin greet
