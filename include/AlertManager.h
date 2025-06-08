#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <mutex>
#include <string>
#include <vector>

class AlertManager {
public:
  AlertManager();
  ~AlertManager() = default;

  void processAlerts(const std::vector<std::string> &alertMessages);

  std::vector<std::string> getLatestAlerts();

  void clearAlerts();

private:
  std::vector<std::string> latestAlerts_;
  mutable std::mutex alertMutex_;
};

#endif
