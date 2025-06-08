#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include <string>
#include <vector>

class AlertManager {
public:
  AlertManager();
  ~AlertManager() = default;

  void processAlerts(const std::vector<std::string> &alertMessages);
};

#endif
