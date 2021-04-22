#include <csignal>
#include <functional>
#include <chrono>
#include <thread>

#include "../include/server.hpp"
#include "../include/jsonParser.hpp"
#include "../include/stopSignal.hpp"

#define SOCKET_INTERFACE AF_INET
#define SOCKET_TYPE SOCK_STREAM
#define SOCKET_PROTOCOL IPPROTO_TCP
#define SOCKET_ADDR_IN INADDR_ANY
#define BUFFER_SIZE 8096
#define SOCKET_PORT 3000

bool running = true;

void stopRunning (int s) {
  std::cout << "[INFO] Received stop signal: " << s << "\n";
  running = false;
}


int main() {
  HttpServer server(SOCKET_PORT);
  // StopSignal stop;

  server.startAccept();

  std::function<void()> stopFunc = std::bind(&HttpServer::stopListening, server);
  StopSignal::setStopServerFunc(stopFunc);

  signal(SIGINT, StopSignal::receiveSignal);

  // while (running) {
  //   using namespace std::chrono_literals;
  //   std::this_thread::sleep_for(100ms);
  // };

  // server.stopListening();

  // JSON hoi;

  return 0;
}