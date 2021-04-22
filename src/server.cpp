#include "../include/server.hpp"

HttpServer::HttpServer(uint16_t port) : HttpServer(AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY, port){}

HttpServer::HttpServer(sa_family_t interface, int type, int protocol, uint32_t listen_addr, uint16_t port) {
  int opt = 1;
  server_listen = true;
  server_listener = nullptr;

  if ((server_fd = socket(interface, type, protocol)) == 0) {
    perror("[HTTP_SERVER] failed to create a socket");
    exit(EXIT_FAILURE);
  }

  // to prevent errors like address already in use after exiting program and restarting
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("[HTTP_SERVER] setsockopt");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = interface;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = listen_addr;

  // bind socket
  if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("[HTTP_SERVER] failed to bind socket");
    exit(EXIT_FAILURE);
  }

  // listen(sockfd, backlog)
  if (listen(server_fd, 3) < 0) {
    perror("[HTTP_SERVER] failed to start listening");
    exit(EXIT_FAILURE);
  }

  // std::function<void(int)> stop = std::bind(&HttpServer::captureStop, this, std::placeholders::_1);
  // std::signal(SIGINT, [stop](int a){ stop(a); });
}

std::thread* HttpServer::startAccept() {
  std::function<void(void)> fun = std::bind(&HttpServer::acceptConnections, this);

  if (server_listener) return server_listener;
  server_listener = new std::thread(fun);

  return server_listener;
}

void HttpServer::stopListening() {
  server_listen = false;
  server_listener->detach();
  std::cout << "\r[INFO] Gracefully stopped listening\n";
  delete server_listener;
}

int HttpServer::getServerFD() {
  return server_fd;
}

struct sockaddr_in HttpServer::getAddress() {
  return server_address;
}

void HttpServer::acceptConnections() {
  std::cout << "[INFO] Started listening\n";
  while (this->server_listen) {
    int addrlen = sizeof(this->server_address);
    int fdc;

    if ((fdc = accept(this->server_fd, (struct sockaddr *)&this->server_address, (socklen_t*)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    } else {
      std::thread aThread([fdc](){ Connection connection(fdc); });
      aThread.detach();
    }
  }
}

void HttpServer::captureStop(int a) {
  stopListening();
  (void)a;
}


// /**
//  * create file descriptor
//  * to create TCP socket use these constants
//  * @param domain AF_INET
//  * @param type SOCK_STREAM
//  * @param protocol IPPROTO_TCP
//  */
// int HttpServer::createSocket(int domain, int type, int protocol) {
//   int opt = 1;

//   if ((server_fd = socket(domain, type, protocol)) == 0) {
//     perror("[HTTP_SERVER] failed to create a socket");
//     exit(EXIT_FAILURE);
//   }

//   // to prevent errors like address already in use after exiting program and restarting
//   if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
//     perror("[HTTP_SERVER] setsockopt");
//     exit(EXIT_FAILURE);
//   }

//   return server_fd;
// }

// /**
//  * create bind and start listening
//  * @param fd returned file descriptor from createSocket()
//  * @param interface usually AF_INET constant
//  * @param port port number to listen on
//  * @param listen_ip on which ip address should we listen, recommended INADDR_ANY constant
//  */
// struct sockaddr_in HttpServer::bindSocket(int fd, sa_family_t interface, in_port_t port, uint32_t listen_ip) {
//   server_address.sin_family = interface;
//   server_address.sin_port = htons(port);
//   server_address.sin_addr.s_addr = listen_ip;

//   // bind socket
//   if (bind(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
//     perror("[HTTP_SERVER] failed to bind socket");
//     exit(EXIT_FAILURE);
//   }

//   // listen(sockfd, backlog)
//   if (listen(fd, 3) < 0) {
//     perror("[HTTP_SERVER] failed to start listening");
//     exit(EXIT_FAILURE);
//   }

//   return server_address;
// }