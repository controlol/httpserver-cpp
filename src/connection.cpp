#include "../include/connection.hpp"

Connection::Connection(int fd) {
  std::cout << "[INFO] created connection\n";

  recvHeaders = Headers();
  sendHeaders = Headers();

  c_fd = fd;
  char buffer[READ_BUFFER_SIZE] = {0}; // create empty buffer to prevent weird data corruption from read
  size_t readBytes = 0;

  while ((readBytes = read(fd, buffer, READ_BUFFER_SIZE))) {
    // std::cout << "buffer: " << "\n" << buffer << "\n";
    recvData.append(buffer);

    // all data has been read
    if (readBytes < READ_BUFFER_SIZE) break;
  }

  // create seperate threads for parsing
  // thread parse headers
  // thread parse json data

  std::thread hThread([&](){ recvHeaders.parseHeaders(recvData); });

  hThread.join();
  // std::cout << this->getRecvHeadersString();

  std::string message("Hello world");

  this->sendHTML(message);
}

void Connection::sendHTML(std::string data) {
  http_header contentType("Content-Type", "text/html");
  sendHeaders.addHeader(contentType);

  http_header contentLength("Content-Length", data.length());
  sendHeaders.addHeader(contentLength);

  std::string message;
  message.append(sendHeaders.getHeaderString());
  message.append(data);

  send(c_fd, message.c_str(), message.length(), 0);
}

void Connection::sendJSON(std::string data) {
  http_header contentType("Content-Type", "application/json");
  sendHeaders.addHeader(contentType);

  http_header contentLength("Content-Length", data.length());
  sendHeaders.addHeader(contentLength);

  // send(c_fd, senddata, strlen(senddata), 0);
}

std::string Connection::getRecvHeadersString() {
  return recvHeaders.getHeaderString();
}