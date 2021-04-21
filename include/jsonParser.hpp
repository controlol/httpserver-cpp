#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <any>
#include <typeinfo> 
#include <sstream>

class JSON {
  public:
    typedef std::map<std::string, std::any> JSON_map; 

  private:
    JSON_map map;

  public:
    JSON(){}
    JSON(JSON_map data);
    JSON_map parse(std::string data);
    void addJson(std::string path, std::any data);
    std::string str(JSON_map data);

  private:
    std::string anyToString(std::any data);
    std::string boolToString(bool b);
    std::string vectorToString(std::vector<std::any> data);
    std::vector<std::string> split(std::string* str, std::string delimiter);
    std::string join(std::vector<std::string>* data, std::string delimiter);
};