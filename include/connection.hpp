#include <string>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include "./headers.hpp"

#define READ_BUFFER_SIZE 2048

class Connection {
  private:
    int c_fd;
    std::string recvData;
    std::string sendData;
    Headers recvHeaders;
    Headers sendHeaders;

  public:
    Connection(int fd);

    void sendHTML(std::string data);
    void sendJSON(std::string data);

  private:
    std::string getRecvHeadersString();
};