#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "AlertManager.h"
#include "DataManager.h"
#include <atomic>
#include <vector>

class UIManager {
public:
  UIManager(DataManager &dataManager, AlertManager &alertManager);
  ~UIManager() = default;

  void run(const std::atomic<bool> &stopFlag);

private:
  void displayDashboard();

  DataManager &dataManager_;
  AlertManager &alertManager_;
};

#endif
