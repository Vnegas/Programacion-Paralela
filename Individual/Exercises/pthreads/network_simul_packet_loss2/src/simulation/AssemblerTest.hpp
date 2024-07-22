/// @author Sebastian Venegas Brenes
/// @copyright Sebastian Venegas Brenes 2023

#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include <iostream>

#include "Assembler.hpp"
#include "NetworkMessage.hpp"
#include "Queue.hpp"

class AssemblerTest
  :public Assembler<NetworkMessage, NetworkMessage> {
  DISABLE_COPY(AssemblerTest);
 protected:
  /// Number of messages received
  int lossProbability = 0;
  int consumerCount = 0;
  /// Delay of consumer to consume a package, negative for max random
  int assemblerDelay = 0;
  /// Number of messages discarded
  size_t discardedMessages = 0;
  int id = 0;
 public:
  /// Constructor
  explicit AssemblerTest(int lossProbability, int consumerCount
      , int assemblerDelay, int id)
    :Assembler() {
    this->lossProbability = lossProbability;
    this->consumerCount = consumerCount;
    this->assemblerDelay = assemblerDelay;
    this->id = id;
  }

  /// Destructor
  virtual ~AssemblerTest() {}
  int run() override;
  /// Consume a product
  void consume(NetworkMessage data) override;
  NetworkMessage createMessageRand() const;
  Queue<NetworkMessage>* getQueue();
};
#endif  // ASSEMBLERTEST_HPP
