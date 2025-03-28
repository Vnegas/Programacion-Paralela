// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>
#include "GoldReqAnalizer.hpp"
#include "TcpServer.hpp"
#include "HttpConnectionHandler.hpp"
#include "HttpDispatcher.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Queue.hpp"

#define DEFAULT_PORT "8080"

class HttpApp;
class HttpRequest;
class HttpConnectionHandler;
class HttpDispatcher;
/**
@brief Implements a minimalist web server.

A web server is a software that listens for client connections,
accept them, waits for requests of resources, and answers the requests. All
communication with clients are following the rules of the HTTP (HyperText
Transfer Protocol).

A HttpServer is a stand-alone object, you do not require to inherit a class.
A web server should be a generic software that may be used for many
applications. A web application is a piece of software that deals with a
specific domain, e.g: a marble shop, a video game, a newspaper, and so on.
Your HttpServer can be connected with an arbitrary number of web
applications. The following code shows how the server can answer requests
of two different applications, one for lottery results and other for pets.

```cpp
int main(int argc, char* argv[]) {
  // Create the web server
  HttpServer httpServer;
  // Create some web applications
  LotteryWebApp lotteryWebApp;
  PetsWebApp petsWebApp;
  // Register the web application(s) with the web server
  httpServer.chainWebApp(&lotteryWebApp);
  httpServer.chainWebApp(&petsWebApp);
  // Start the web server
  return httpServer.run(argc, argv);
}
```

In the previous code, when a request arrives, the server asks the lottery
application to examine the request. The lottery application will study the
HttpRequest fields (e.g: URI) to determine if the request is about lottery.
In such case, the web application returns true to the server indicating it
managed the request. If the application returns false, indicates the server
that the request was not intended for the lottery application, and the server
repeats the process with the following application in the chain: the pets
application. If no application manages the request, a 404 Not-found response
is sent to the client.
*/
class HttpServer : public TcpServer {
  DISABLE_COPY(HttpServer);

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;
  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;
  /// Chain of registered web applications. Each time an incoming HTTP request
  /// is received, the request is provided to each application of this chain.
  /// If an application detects the request is for it, the application will
  /// call the httpResponse.send() and the chain stops. If no web app serves
  /// the request, the not found page will be served.
  std::vector<HttpApp*> applications;

  /// Maximum number of connections allowed simultaneously
  /// is provided by the user in the arguments otherwise
  /// maximum number of nucleus in the machine are used
  int64_t max_connection = std::thread::hardware_concurrency();

  /// Generic queue used to store the sockets generated
  Queue<Socket>* serverQueue;

  /// Array that stores objects of type HttpConnectionHandler
  /// these objects will later consume objects sockets from
  /// server's queue and do the job requiered to process them
  std::vector <HttpConnectionHandler*> handlers;

  /// Object that will dispatch requests and responses to each
  /// application analyzer request
  HttpDispatcher* dispatcher;

 public:
  /// Get access to the unique instance of this Singleton class.
  static HttpServer& getInstance();
  /// Registers a web application to the chain
  void chainWebApp(HttpApp* application);
  /// Start the web server for listening client connections and HTTP requests
  int run(int argc, char* argv[]);
  /// Stop the web server. The server may stop not immediately. It will stop
  /// for listening further connection requests at once, but pending HTTP
  /// requests that are enqueued will be allowed to finish
  void stop();
  /// Infinetelly listen for client connection requests and accept all of them.
  /// For each accepted connection request, the virtual onConnectionAccepted()
  /// will be called. Inherited classes must override that method
  void listenForever(const char* port);

 protected:
  /// Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  bool analyzeArguments(int argc, char* argv[]);
  /// Start all registered applications, given them a chance to build their
  /// data structures just before starting to run
  void startApps();
  /// Stop all running applications, given them a chance to clean their data
  /// structures
  void stopApps();
  /// This method is called each time a client connection request is accepted.
  void handleClientConnection(Socket& client) override;

 private:
  /// Constructor
  HttpServer();
  /// Destructor
  ~HttpServer();
};

#endif  // HTTPSERVER_H
