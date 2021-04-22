#include "../include/stopSignal.hpp"

void jemoeder(){std::cout << "hihi exiting\n";}

std::function<void()> StopSignal::stopServerFunc = jemoeder;

void StopSignal::setStopServerFunc(std::function<void()> f) {
  stopServerFunc = f;
}

void StopSignal::receiveSignal(int s) {
  std::cout << "[INFO] Received stop signal: " << s << "\n";
  stopServerFunc();
}