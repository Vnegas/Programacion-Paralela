/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "AssemblerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: prodcons packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "  prob_loss   probability to lose a package\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for ( ProducerTest* producer : this->producers )
    delete producer;
  delete this->dispatcher;
  delete this->assembler;
  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
  delete this->sharedData;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  // Create objects for the simulation
  // Create the shared data for producers
  this->sharedData = new SharedData(this->packageCount);
  // Create each producer
  this->producers.resize(this->producerCount);
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index] = new ProducerTest(this->packageCount
      , this->productorDelay, this->consumerCount+1, this->sharedData);
    /*this->producers.push_back(new ProducerTest(this->packageCount
      , this->productorDelay, this->consumerCount+1, this->sharedData));*/
    assert(this->producers[index]);
  }
  this->dispatcher = new DispatcherTest(this->dispatcherDelay);
  this->dispatcher->createOwnQueue();
  // Create assembler
  this->assembler = new AssemblerTest(this->probLoss, this->consumerCount
    , this->productorDelay, this->consumerCount+1);
  this->assembler->createOwnQueue();
  // Create each consumer
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue();
  }

  // Communicate simulation objects
  // Producers push network messages to the dispatcher queue
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->setProducingQueue(this->dispatcher
      ->getConsumingQueue());
  }
  // Assembler push network messages to the dispatcher queue
  this->assembler->setProducingQueue(this->dispatcher->getConsumingQueue());
  // Dispatcher delivers to each consumer, and they should be registered
  for ( size_t index = 0; index < this->consumerCount + 1; ++index ) {
    if (index < this->consumerCount) {
      this->dispatcher->registerRedirect(index + 1
        , this->consumers[index]->getConsumingQueue());
    } else {
      this->dispatcher->registerRedirect(this->consumerCount + 1
        , this->assembler->getConsumingQueue());
    }
  }

  // Start the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->startThread();
  }
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount + 1; ++index ) {
    if (index < this->consumerCount) {
      this->consumers[index]->startThread();
    } else {
      this->assembler->startThread();
    }
  }

  // Wait for objets to finish the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->waitToFinish();
  }
  this->dispatcher->waitToFinish();
  for ( size_t index = 0; index < this->consumerCount + 1; ++index ) {
    if (index < this->consumerCount) {
      this->consumers[index]->waitToFinish();
    } else {
      this->assembler->waitToFinish();
    }
  }

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 6 + 1 arguments are mandatory
  if ( argc != 8 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);
  this->probLoss = std::stod(argv[index++]);

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
