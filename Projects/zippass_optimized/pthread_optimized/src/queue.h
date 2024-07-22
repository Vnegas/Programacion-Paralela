//  Copyright 2023 Sebastian Venegas

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

/**
 * @brief struct that acts like a node or element of queue, contains data and a
 * pointer to next element
 */
typedef struct queue_node {
  char* data;
  struct queue_node* next;
} queue_node_t;

/**
 * @brief struct that acts like a queue, contains a head and a tail
 */
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @brief initializes queue and its attributes
 * @param queue 
 * @return int, error code
 */
int queue_init(queue_t* queue);
/**
 * @brief destroys/free queue and memory allocated
 * @param queue
 * @return int, error code
 */
int queue_destroy(queue_t* queue);
/**
 * @brief finds if queue is empty or if it has elements, it is thread-safe
 * @param queue
 * @return true if queue is empty
 * @return false if queue is not empty
 */
bool queue_is_empty(queue_t* queue);
/**
 * @brief adds (push) new element to the queue, it is thread-safe
 * @param queue
 * @param data
 * @param size
 * @return int, error code
 */
int queue_enqueue(queue_t* queue, char* data, size_t size);
/**
 * @brief pops element from queue and deletes it, it is thread-safe
 * @param queue 
 * @param data 
 * @return int, error code
 */
int queue_dequeue(queue_t* queue, char* data);
/**
 * @brief clears queue, empties queue, it is thread-safe
 * @param queue
 */
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
