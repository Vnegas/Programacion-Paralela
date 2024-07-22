// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Queue.hpp"
#include "Socket.hpp"

const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n";

HttpServer& HttpServer::getInstance() {
  static HttpServer server;
  return server;
}

HttpServer::HttpServer() {
}

HttpServer::~HttpServer() {
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::run(int argc, char* argv[]) {
  bool stopApps = false;
  try {
    if (this->analyzeArguments(argc, argv)) {
      this->serverQueue = new Queue<Socket>();
      this->dispatcher = new HttpDispatcher(this->applications
        , this->max_connection);
      this->dispatcher->createOwnQueue();
      /// Resizes the array to the number of max connections
      this->handlers.resize(this->max_connection);
      for (int index = 0; index < this->max_connection; ++index) {
        this->handlers[index] = new HttpConnectionHandler();
        this->handlers[index]->setConsumingQueue(this->serverQueue);
        this->handlers[index]->setProducingQueue
          (this->dispatcher->getConsumingQueue());
      }
      // Start the log service
      Log::getInstance().start();

      // Start all web applications
      this->startApps();
      stopApps = true;

      // Matches dispatcher with each application given it's ID
      for (size_t i = 0; i < this->applications.size(); i++) {
        this->dispatcher->registerRedirect(this->applications[i]->ID,
        this->applications[i]->getQueue());
      }

      // Start waiting for connections
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
        + " port " + std::to_string(address.getPort()));

      // Accept all client connections. The main process will get blocked
      // running this method and will not return. When HttpServer::stop() is
      // called from another execution thread, an exception will be launched
      // that stops the acceptAllConnections() invocation and enters in the
      // catch below. Then, the main thread can continue stopping apps,
      /// finishing the server and any further cleaning it requires.
      for (int index = 0; index < this->max_connection; ++index) {
        this->handlers[index]->startThread();
      }
      this->dispatcher->startThread();
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
    // Send stop condition to serverQueue to be read from every handler
    for (int index = 0; index < this->max_connection+2; ++index) {
      this->serverQueue->enqueue(Socket());
    }
    // Waits to every handler to finish its job (read stop condition)
    for (int index = 0; index < this->max_connection; ++index) {
      this->handlers[index]->waitToFinish();
    }
    // Waits for dispatcher
    this->dispatcher->waitToFinish();
    // Delete memory reserved for every handler (thread)
    for (int index = 0; index < this->max_connection; ++index) {
      delete this->handlers[index];
    }
    // Delete memory reserved for serverQueue (Queue)
    delete this->serverQueue;
    // Deletes server dispatcher
    delete this->dispatcher;

    // If applications were started, stops them
    if (stopApps) {
      this->stopApps();
    }
    // Stop the log service
    Log::getInstance().stop();
  }

  return EXIT_SUCCESS;
}

void HttpServer::startApps() {
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->start();
  }
}

void HttpServer::stopApps() {
  // Stop web applications. Give them an opportunity to clean up
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->stop();
  }
}

void HttpServer::stop() {
  // Stop listening for incoming client connection requests. When stopListing()
  // method is called -maybe by a secondary thread-, the web server -running
  // by the main thread- will stop executing the acceptAllConnections() method.
  this->stopListening();
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 3) {
    this->port = argv[1];
    this->max_connection = std::stoi(argv[2]);
  }

  return true;
}

void HttpServer::handleClientConnection(Socket& client) {
  this->serverQueue->enqueue(client);
}
