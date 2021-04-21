#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <functional>
#include <csignal>

#include "./connection.hpp"

class HttpServer {
  private:
    int server_fd;
    struct sockaddr_in server_address;
    bool server_listen;
    std::thread *server_listener;
    // static running = true;

  public:
    HttpServer(sa_family_t interface, int type, int protocol, uint32_t listen_addr, uint16_t port);
    HttpServer(uint16_t port);

    std::thread* startAccept();
    void stopListening();

    int getServerFD();
    struct sockaddr_in getAddress();

  private:
    void acceptConnections();
    void captureStop(int a);
};