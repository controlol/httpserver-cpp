#include <vector>
#include <string>
#include <iostream>
#include <memory> // for memcpy
#include <climits> // for SIZE_MAX
#include <map>
#include <sstream>

struct http_header {
  std::string name;
  std::string value;
  inline http_header(){}
  inline http_header(std::string name, std::string value) {
    this->name = name;
    this->value = value;
  }
  inline http_header(std::string name, const char* value) {
    std::string valueStr(value);
    this->name = name;
    this->value = valueStr;
  }
  inline http_header(std::string name, size_t value) {
    this->name = name;
    this->value = std::to_string(value);
  }
  inline http_header(const char* name, std::string value) {
    std::string nameStr(name);
    this->name = nameStr;
    this->value = value;
  }
  inline http_header(const char* name, const char* value) {
    std::string nameStr(name);
    std::string valueStr(value);
    this->name = nameStr;
    this->value = valueStr;
  }
  inline http_header(const char* name, size_t value) {
    std::string nameStr(name);
    this->name = nameStr;
    this->value = std::to_string(value);
  }
  inline http_header(std::vector<std::string> header) {
    name = header[0];
    value = header[1];
  }
};

class Headers {
  public:
    typedef std::map<std::string, std::string> HeaderMap;

  private:
    HeaderMap headers;
    // std::vector<http_header> headers;
    int http_version; // 10x normal version would be, 1.1 => 11
    int http_status;
    std::string http_method;
    std::string http_route;

  public:
    Headers();

    Headers::HeaderMap getHeaders();
    std::string getHeaderString();
    size_t getHeaderCount();

    int setHTTPversion(float version);
    int setHTTPstatus(int status);
    int parseHeaders(std::string str);
    void setContentLength(size_t length);
    void addHeader(http_header header);
    void deleteHeader(std::string headerName);

  private:
    std::vector<std::string> split(std::string str, std::string delimiter);
    std::vector<std::string> splitOnce(std::string str, std::string delimiter);
    void parseVersion(std::string str);
    void parseMethod(std::string str);
    void parseRoute(std::string str);
    size_t getHeadersCharacterCount();
    std::string getExistingHeader(std::string headerName);
    std::string getStatusString();
    std::string getVersionString();
};