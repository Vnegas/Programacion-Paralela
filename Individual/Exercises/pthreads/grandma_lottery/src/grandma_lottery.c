//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

/**
 * @brief generates a pseudo-random number using function rand_r.
 * @param NULL
 * @return number cast as void to simulate a memory address.
 */
void* generate_random_number_false_direction(void* data);
/**
 * @brief generates a pseudo-random number using function rand_r and saving it
 * in heap memory with malloc.
 * @param NULL
 * @return memory address of number.
 */
void* generate_random_number_heap(void* data);

int main(void) {
    pthread_t thread_number1;
    pthread_t thread_number2;
    int error_thread1 = pthread_create(&thread_number1, /*attr*/ NULL
        , generate_random_number_false_direction, /*arg*/ NULL);
    if (error_thread1 == EXIT_SUCCESS) {
        void* random_number = NULL;
        pthread_join(thread_number1, &random_number);
        printf("First lottery ticket:  %zu\n", (size_t)random_number);
    } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        return EXIT_FAILURE;
    }

    int error_thread2 = pthread_create(&thread_number2, /*attr*/ NULL
        , generate_random_number_heap, /*arg*/ NULL);
    if (error_thread2 == EXIT_SUCCESS) {
        void* random_number = NULL;
        pthread_join(thread_number2, &random_number);
        size_t* x = (size_t*)random_number;
        printf("Second lottery ticket: %zu\n", *x/*(size_t)random_number*/);
        free(random_number);
    } else {
        fprintf(stderr, "Error: could not create third thread\n");
        return EXIT_FAILURE;
    }
}

void* generate_random_number_false_direction(void* data) {
    (void)data;
    unsigned int my_seed = pthread_self() ^ time(NULL) ^ getpid();

    long number = (long)(rand_r(&my_seed) % 99)+1;
    return (void*)number;
}

void* generate_random_number_heap(void* data) {
    (void)data;
    // hacer malloc o calloc para guardar en heap
    unsigned int *number = (unsigned int *) malloc(sizeof(unsigned int));
    unsigned int my_seed = pthread_self() * time(NULL) * getpid();
    *number = (rand_r(&my_seed) % 99)+1;
    return (void*)number;
}
