//  Copyright 2023 Sebastian Venegas

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

/**
 * @brief struct of the node
*/
typedef struct queue_node {
  size_t data;
  struct queue_node* next;
} queue_node_t;

/**
 * @brief Thread safe queue
 * @details Doesnt need locking
 */
typedef struct {
  pthread_mutex_t can_access_queue;
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

/**
 * @brief Initializes the queue
 */
int queue_init(queue_t* queue);
/**
 * @brief Destroys the queue, removing all nodes and frees memory
 */
int queue_destroy(queue_t* queue);
/**
 * @brief Checks if the queue is empty
 * @return true is empty
 * @return false if not
 */
bool queue_is_empty(queue_t* queue);
/**
 * @brief enqueues a new node
 * @param queue
 * @param data
 * @return error code, 0 if success
 */
int queue_enqueue(queue_t* queue, const size_t data);
/**
 * @brief dequeues a node
 * @param queue
 * @param data
 * @return error code, 0 if success
 */
int queue_dequeue(queue_t* queue, size_t* data);
/**
 * @brief clears queue, removes all nodes
 * @param queue
 */
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
