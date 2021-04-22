#include "../include/jsonParser.hpp"

JSON::JSON(JSON_map data) {
  map = data;
}

JSON::JSON_map JSON::parse(std::string data) {
  
}

void JSON::addJson(std::string path, std::any data) {
  std::vector<std::string> pathArr = split(&path, ".");
}


std::string JSON::str(JSON_map data) {
  std::string dataStr("{ ");

  unsigned int i = 0;
  for (auto const& [key, value] : data) {
    dataStr.append("\"" + key + "\": " + anyToString(value));
    if (i < data.size()) dataStr.append(", ");
    i++;
  }

  return dataStr;
}

std::string JSON::anyToString(std::any data) {
  std::string dataStr;

  if (data.type() == typeid(int)) dataStr.append(std::to_string(std::any_cast<int>(data)));
  if (data.type() == typeid(float)) dataStr.append(std::to_string(std::any_cast<float>(data)));
  if (data.type() == typeid(std::string)) dataStr.append("\"" + std::any_cast<std::string>(data) + "\"");
  if (data.type() == typeid(const char*)) {
    dataStr.append("\"");
    dataStr.append(std::any_cast<const char*>(data));
    dataStr.append("\"");
  }
  if (data.type() == typeid(bool)) dataStr.append(boolToString(std::any_cast<bool>(data)));
  if (data.type() == typeid(std::vector<std::any>)) dataStr.append(vectorToString(std::any_cast<std::vector<std::any>>(data)));
  if (data.type() == typeid(JSON::JSON_map)) dataStr.append(str(std::any_cast<JSON::JSON_map>(data)));

  return dataStr;
}

std::string JSON::boolToString(bool b) {
  return b ? "true" : "false";
}

// include some kind of depth so we can add nextlines and appropriate spaces
std::string JSON::vectorToString(std::vector<std::any> data) {
  std::string dataStr("[ ");

  unsigned int i = 0;
  for (auto& d : data) {
    dataStr.append("\"" + anyToString(d) + "\"");
    if (i < data.size()) dataStr.append(", ");
    i++;
  }

  dataStr.append(" ]");

  return dataStr;
}

/**
 * return string vector seperated by delimiter
 * @param str
 * @param delimiter
 */
std::vector<std::string> JSON::split(std::string* str, std::string delimiter) {
  std::vector<std::string> data;

  size_t delimiter_length = delimiter.length();
  size_t i = 0;

  while(1) {
    size_t pos1 = i;
    size_t pos2 = str->find(delimiter, i);
    size_t len = pos2 - pos1;

    data.push_back(str->substr(pos1, len));

    if (pos2 == SIZE_MAX) break;
    i = pos2 + delimiter_length;
  }

  return data;
}

std::string JSON::join(std::vector<std::string>* data, std::string delimiter) {
  std::string dataStr;

  for (std::vector<std::string>::const_iterator p = data->begin(); p != data->end(); ++p) {
    dataStr += *p;
    if (p != data->end() - 1) dataStr += delimiter;
  }

  return dataStr;
}