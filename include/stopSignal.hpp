#include <string>
#include <functional>
#include <csignal>
#include <iostream>

class StopSignal {
  private:
    static std::function<void()> stopServerFunc;

  public:
    static void setStopServerFunc(std::function<void()> f);
    static void receiveSignal(int s);

  private:

};