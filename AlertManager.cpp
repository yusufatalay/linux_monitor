#include "AlertManager.h"
#include <iostream>
#include <mutex>

AlertManager::AlertManager() {}

void AlertManager::processAlerts(
    const std::vector<std::string> &alertMessages) {
  if (!alertMessages.empty()) {
    std::lock_guard<std::mutex> lock(alertMutex_);
    latestAlerts_ = alertMessages;
  }
}

std::vector<std::string> AlertManager::getLatestAlerts() {
  std::lock_guard<std::mutex> lock(alertMutex_);

  std::vector<std::string> alerts_copy = latestAlerts_;
  latestAlerts_.clear();

  return latestAlerts_;
}
