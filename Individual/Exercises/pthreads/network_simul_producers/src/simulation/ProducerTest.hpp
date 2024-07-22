/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef PRODUCERTEST_HPP
#define PRODUCERTEST_HPP

#include "NetworkMessage.hpp"
#include "Producer.hpp"
#include "SharedData.hpp"

/**
 * @brief A productor class example
 * Produces network messages and push them to the queue
 */
class ProducerTest : public Producer<NetworkMessage> {
  DISABLE_COPY(ProducerTest);

 protected:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;
  /// Number of packages produced
  size_t producedMessages = 0;
  // Shared data between producers
  SharedData* sharedData = nullptr;

 public:
  /// Constructor
  ProducerTest(size_t packageCount, int productorDelay, size_t consumerCount
    , SharedData* sharedData);
  /// Do the message production in its own execution thread
  int run() override;
  /// Creates a simulation message to be sent by the network
  NetworkMessage createMessage(size_t index) const;
};

#endif  // PRODUCERTEST_HPP
