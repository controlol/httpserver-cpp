#include "../include/stopSignal.hpp"

void StopSignal::setStopServerFunc(std::function<void()> f) {
  StopSignal::stopServerFunc = f;
}

void StopSignal::receiveSignal(int s) {
  std::cout << "[INFO] Received stop signal: " << s << "\n";
  StopSignal::stopServerFunc();
}