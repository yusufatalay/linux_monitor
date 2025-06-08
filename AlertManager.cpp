#include "AlertManager.h"
#include <iostream>

AlertManager::AlertManager() {}

void AlertManager::processAlerts(
    const std::vector<std::string> &alertMessages) {
  if (!alertMessages.empty()) {
    std::cout << "\n!! ALERTS !! " << std::endl;
    for (const auto &message : alertMessages) {
      std::cout << "- " << message << std::endl;
    }

    std::cout << "!!!!!!!!!!!!!!!!" << std::endl;
  }
}
