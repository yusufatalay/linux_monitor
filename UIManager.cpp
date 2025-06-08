#include "UIManager.h"
#include "AlertManager.h"
#include "DataManager.h"
#include "MetricData.h"
#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <thread>
#include <variant>
#include <vector>

UIManager::UIManager(DataManager &dataManager, AlertManager &alertManager)
    : dataManager_(dataManager), alertManager_(alertManager) {}

void UIManager::displayDashboard() {
  // clear the console and move the cursor to position 1,1
  std::cout << "\033[2J\033[1;1H";

  std::cout << "--- Linux System Monitor (SysMon) ---" << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "METRICS:" << std::endl;

  std::vector<MetricData> currentMetrics = dataManager_.getLatestMetrics();

  if (currentMetrics.empty()) {
    std::cout << "\tWaiting for initial metric data..." << std::endl;
  } else {
    for (const auto &data : currentMetrics) {
      std::cout << "\t" << std::left << std::setw(25) << data.name << ": ";
      std::cout << std::right << std::setw(8) << std::fixed
                << std::setprecision(2);

      std::visit(
          [&](const auto &val) {
            if (std::holds_alternative<double>(data.value)) {
              std::cout << std::get<double>(data.value);
            } else if (std::holds_alternative<long>(data.value)) {

            } else if (std::holds_alternative<std::string>(data.value)) {
              std::cout << std::get<std::string>(data.value);
            }
          },
          data.value);

      std::cout << "\t" << data.unit << std::endl;
    }
  }

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "ALERTS:" << std::endl;

  std::vector<std::string> lastAlerts = alertManager_.getLatestAlerts();
  if (lastAlerts.empty()) {
    std::cout << "\tNo active alerts." << std::endl;
  } else {
    for (const auto &alert : lastAlerts) {
      std::cout << "\t\033[1;31m" << alert << "\033[0m"
                << std::endl; // red warning
    }
  }

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "(Press Ctrl+C to exit)" << std::endl;
  std::cout << std::flush;
}

void UIManager::run(const std::atomic<bool> &stopFlag) {
  while (!stopFlag) {
    displayDashboard();

    // refresh ui for every half a second
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  std::cout << "\nUI thread shutting down." << std::endl;
}
