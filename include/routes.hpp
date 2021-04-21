#include <string>
#include <map>
#include <functional>

class Route {
  public:
    typedef std::function<std::string(void)> RouteContent;
    typedef std::map<std::string, Route*> RouteMap;

  private:
    RouteMap children;
    Route* parent;

  public:
    Route();
    Route(Route* parent, RouteContent newfunc);

    void addRoute(std::string path, Route* newRoute);

    std::string callFunction();

  private:

};