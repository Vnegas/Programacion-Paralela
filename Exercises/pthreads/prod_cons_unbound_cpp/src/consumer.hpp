//  Copyright 2023 Sebastian Venegas

#ifndef CONSUMER_H
#define CONSUMER_H

void* consume(void* data);

#include "Thread.hpp"
#include "disableCopy.hpp"
template <typename dataType>
/**
 * @brief The consumer class, it inherits from thread<dataType> and overrides 
 * the process method
 */
class consumer: public thread<dataType> {
 public:
  consumer() {
    this -> myThread = nullptr;
  }

  DISABLE_COPY(consumer);
  /**
   * @brief for a consumer, the process method is to consume the data until a
   * stop condition is met
   * @param data 
  */
  void process(dataType data) override {
    simulationData* simulationData = (struct simulationData*) data;

    while (true) {
      simulationData->can_access_consumed_count.lock();
      if (simulationData->consumed_count < simulationData->unit_count) {
        ++simulationData->consumed_count;
      } else {
        simulationData->can_access_consumed_count.unlock();
        break;
      }
      simulationData->can_access_consumed_count.unlock();

      sem_wait(&simulationData->can_consume);

      size_t value = 0;

      simulationData->threadQueue.dequeue(&value);
      std::cout << "\tConsuming " << value << std::endl;

      usleep(1000 * random_between(simulationData->consumer_min_delay,
      simulationData->consumer_max_delay));
    }
  }
};

#endif  // CONSUMER_H
