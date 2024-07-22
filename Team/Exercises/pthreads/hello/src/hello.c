//  Copyright 2023 Sebastian Venegas

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief prints text from a secondary thread
 * @param NULL
 * @return NULL
 */
void* greet(void* data);


int main(void) {
  // crear hilo(greet)
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  // if hilo creado correctamente
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    printf("Hello from main thread\n");
    // llamada a greet
    pthread_join(thread, /*value_ptr*/ NULL);
    // else
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // fin

// greet:
void* greet(void* data) {
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}  // fin greet
