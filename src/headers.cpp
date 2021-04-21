#include "../include/headers.hpp"

Headers::Headers() {
  http_version = 10;
  http_status = 200;
  http_method = "";
}

// VIEW
Headers::HeaderMap Headers::getHeaders() {
  return headers;
}

std::string Headers::getHeaderString() {
  std::stringstream headerStr;

  headerStr << getVersionString() << " " << getStatusString() << "\r\n";

  for (auto const& [key, value] : headers) headerStr << key << ": " << value << "\r\n";

  headerStr << "\r\n";

  return headerStr.str();
}

// get the amount of headers stored
size_t Headers::getHeaderCount() {
  return headers.size();
}

// CONTROLLER

/**
 * returns 0 if successful
 * @param version http version
 */
int Headers::setHTTPversion(float version) {
  if (
    version == 1.0f ||
    version == 1.1f ||
    version == 2.0f
  ) {
    http_version = (int)(version * 10);
    return 0;
  }

  return 1;
}

/**
 * returns 0 if successful
 * @param status http status code
 */
int Headers::setHTTPstatus(int status) {
  if (
    (
      status >= 100 && 
      status <= 103
    ) ||
    (
      status >= 200 &&
      status <= 208
    ) ||
    status == 226 ||
    (
      status >= 300 &&
      status <= 304
    ) ||
    status == 307 ||
    status == 308 ||
    (
      status >= 400 &&
      status <= 418
    ) ||
    (
      status >= 421 &&
      status <= 426
    ) ||
    status == 428 ||
    status == 429 ||
    status == 431 ||
    status == 451 ||
    (
      status >= 500 &&
      status <= 508
    ) ||
    status == 510 ||
    status == 511
  ) {
    http_status = status;
    return 0;
  }

  return 1;
}

/**
 * parse headers in str and add them to the class
 * returns 0 if successful
 * @param {std::string} str the datastring to be parsed
 */
int Headers::parseHeaders(std::string str) {
  std::string strHeaders = splitOnce(str, "\r\n\r\n")[0];

  std::vector<std::string> lines = split(str, "\r\n");

  parseMethod(lines[0]);
  parseRoute(lines[0]);
  parseVersion(lines[0]);

  if (http_method == "") {
    std::cout << "[ERROR] could not parse http_method\n";
    return 1;
  }
  if (http_version == 0) {
    std::cout << "[ERROR] could not parse http_version\n";
    return 1;
  }

  lines.erase(lines.begin());
  lines.pop_back();
  lines.pop_back();

  for (auto& l : lines) {
    std::vector<std::string> headerstrings = splitOnce(l, ": ");
    if (headerstrings.size() < 2) {
      std::cout << "[ERROR] parsed bad header\n" << headerstrings[0] << "\n";
      return 1;
    }
    http_header header(headerstrings);
    headers[headerstrings[0]] = headerstrings[1];
  }

  return 0;
}

/**
 * set the Content-Length header
 * should be the amount of characters send as data part of the response
 * @param {size_t} length 
 */
void Headers::setContentLength(size_t length) {
  headers["Content-Length"] = std::to_string(length);
}

/**
 * add header to the class
 * @param {http_header} header
 */
void Headers::addHeader(http_header header) {
  headers[header.name] = header.value;
}

/**
 * Delete header if it exists
 * @param {std::string} headerName name of the header that should be deleted
 */
void Headers::deleteHeader(std::string headerName) {
  headers.erase(headerName);
}

// PRIVATE
/**
 * return string vector seperated by delimiter
 * @param str
 * @param delimiter
 */
std::vector<std::string> Headers::split(std::string str, std::string delimiter) {
  std::vector<std::string> data;

  size_t delimiter_length = delimiter.length();

  size_t i = 0;
  while(1) {
    size_t pos1 = i;
    size_t pos2 = str.find(delimiter, i);
    size_t len = pos2 - pos1;

    data.push_back(str.substr(pos1, len));

    if (pos2 == SIZE_MAX) break;
    i = pos2 + delimiter_length;
  }

  return data;
}

/**
 * return string vector seperated once by delimiter
 * @param str
 * @param delimiter
 */
std::vector<std::string> Headers::splitOnce(std::string str, std::string delimiter) {
  std::vector<std::string> data;

  size_t pos2 = str.find(delimiter);

  if (pos2 != SIZE_MAX) {
    data.push_back(str.substr(0, pos2));
    data.push_back(str.substr(pos2 + delimiter.length()));
  } else {
    data.push_back(str);
  }

  return data;
}

void Headers::parseVersion(std::string str) {
  size_t pos = str.find("HTTP/") + 5;
  if (pos > str.length()) {
    http_version = 0;
  } else {
    std::string versionStr = str.substr(pos);

    http_version = (int)(std::stof(versionStr) * 10);
  }
}

void Headers::parseMethod(std::string str) {
  std::string method = splitOnce(str, " ")[0];

  if (method == "GET") http_method = "GET";
  else if (method == "POST") http_method = "POST";
  else if (method == "PUT") http_method = "PUT";
  else if (method == "DELETE") http_method = "DELETE";
  else if (method == "HEAD") http_method = "HEAD";
  else if (method == "CONNECT") http_method = "CONNECT";
  else if (method == "OPTIONS") http_method = "OPTIONS";
  else if (method == "TRACE") http_method = "TRACE";
  else if (method == "PATCH") http_method = "PATCH";
}

void Headers::parseRoute(std::string str) {
  http_route = split(str, " ")[1];
}

/**
 * count the total characters the http header will use
 */
size_t Headers::getHeadersCharacterCount() {
  size_t cc = 0;

  cc += getVersionString().length();
  cc += getStatusString().length();

  for (auto const& [key, value] : headers) cc += key.length() + value.length() + 4; // the 4 is for ": " and "\r\n"

  cc += 2; // for the last "\r\n"

  return cc;
}

/**
 * return a pointer to the header if it exists so it can be modified
 * return NULL of the header does not exists
 * @param {std::string} headerName name of the header
 */
std::string Headers::getExistingHeader(std::string headerName) {
  if (headers.find(headerName) != headers.end()) {
    return headers[headerName];
  }
  return std::string("");
}

std::string Headers::getStatusString() {
  std::string status;

  switch (http_status) {
    case 100: 
      status.append("100 Continue");
      break;
    case 101: 
      status.append("101 Switching Protocol");
      break;
    case 102: 
      status.append("102 Processing"); // webdav
      break;
    case 103: 
      status.append("103 Early Hints");
      break;
    case 200: 
      status.append("200 OK");
      break;
    case 201: 
      status.append("201 Created");
      break;
    case 202: 
      status.append("202 Accepted");
      break;
    case 203: 
      status.append("203 Non-Authoritative Information");
      break;
    case 204: 
      status.append("204 No Content");
      break;
    case 205: 
      status.append("205 Reset Content");
      break;
    case 206: 
      status.append("206 Partial Content");
      break;
    case 207: 
      status.append("207 Multi-Status"); // webdav
      break;
    case 208: 
      status.append("208 Already Reported"); // webdav
      break;
    case 226: 
      status.append("226 IM Used");
      break;
    case 300: 
      status.append("300 Multiple Choice");
      break;
    case 301: 
      status.append("301 Moved Permanently");
      break;
    case 302: 
      status.append("302 Found");
      break;
    case 303: 
      status.append("303 See Other");
      break;
    case 304: 
      status.append("304 Not Modified");
      break;
    case 307: 
      status.append("307 Temporary Redirect");
      break;
    case 308: 
      status.append("308 Permanent Redirect");
      break;
    case 400: 
      status.append("400 Bad Request");
      break;
    case 401: 
      status.append("401 Unauthorized");
      break;
    case 402: 
      status.append("402 Payment Required");
      break;
    case 403: 
      status.append("403 Forbidden");
      break;
    case 404: 
      status.append("404 Not Found");
      break;
    case 405: 
      status.append("405 Method Not Allowed");
      break;
    case 406: 
      status.append("406 Not Acceptable");
      break;
    case 407: 
      status.append("407 Proxy Authentication Required");
      break;
    case 408: 
      status.append("408 Request Timeout");
      break;
    case 409: 
      status.append("409 Conflict");
      break;
    case 410: 
      status.append("410 Gone");
      break;
    case 411: 
      status.append("411 Length Required");
      break;
    case 412: 
      status.append("412 Precondition Failed");
      break;
    case 413: 
      status.append("413 Payload Too Large");
      break;
    case 414: 
      status.append("414 URI Too Long");
      break;
    case 415: 
      status.append("415 Unsupported Media Type");
      break;
    case 416: 
      status.append("416 Range Not Satisfied");
      break;
    case 417: 
      status.append("417 Expectation Failed");
      break;
    case 418: 
      status.append("418 I'm a Teapot");
      break;
    case 421: 
      status.append("421 Misdirected Request");
      break;
    case 422: 
      status.append("422 Unprocessable Entity"); // webdav
      break;
    case 423: 
      status.append("423 Locked"); // webdav
      break;
    case 424: 
      status.append("424 Failed Dependency"); // webdav
      break;
    case 425: 
      status.append("425 Too Early");
      break;
    case 426: 
      status.append("426 Upgrade Required");
      break;
    case 428: 
      status.append("428 Precondition Required");
      break;
    case 429: 
      status.append("429 Too Many Requests");
      break;
    case 431: 
      status.append("431 Request Header Fields Too Large");
      break;
    case 451: 
      status.append("451 Unavailable For Legel Reasons");
      break;
    case 500: 
      status.append("500 Internal Server Error");
      break;
    case 501: 
      status.append("501 Not Implemented");
      break;
    case 502: 
      status.append("502 Bad Gateway");
      break;
    case 503: 
      status.append("503 Service Unavailable");
      break;
    case 504: 
      status.append("504 Gateway Timeout");
      break;
    case 505: 
      status.append("505 HTTP Version Not Supported");
      break;
    case 506: 
      status.append("506 Variant Also Negotiates");
      break;
    case 507: 
      status.append("507 Insufficient Storage"); // webdav
      break;
    case 508: 
      status.append("508 Loop Detected"); // webdav
      break;
    case 510: 
      status.append("510 Not Extended");
      break;
    case 511: 
      status.append("511 Network Authentication Required");
      break;
    default:
      break;
  }

  // deprecated
  // if (status == 305) status.append("305 Use Proxy");
  // if (status == 306) status.append("306 unused");

  return status;
}

std::string Headers::getVersionString() {
  std::string version("HTTP/");

  switch (http_version) {
    case 10:
      version.append("1.0");
      break;
    case 11:
      version.append("1.1");
      break;
    case 20:
      version.append("2.0");
      break;
    default:
      break;
  }

  return version;
}