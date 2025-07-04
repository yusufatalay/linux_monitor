#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "AbstractMetricCollector.h"
#include "AlertManager.h"
#include "ConfigManager.h"
#include "CpuCollector.h"
#include "DataManager.h"
#include "DiskCollector.h"
#include "MemoryCollector.h"
#include "MetricData.h"
#include "Rule.h"
#include "RuleEngine.h"
#include "UIManager.h"

std::atomic<bool> stopFlag(false);

void signalHandler(int signum) {
  std::cout << "\nInterrupts signal (" << signum
            << ") received. Shutting down..." << std::endl;
  stopFlag = true;
}

void collectorWorker(AbstractMetricCollector *collector,
                     DataManager &dataManager,
                     const std::atomic<bool> &stopFlag, int interval) {
  while (!stopFlag) {
    MetricData data = collector->collect();
    dataManager.updateMetric(data);
    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }

  std::cout << collector->getName() << " collector thread shutting down."
            << std::endl;
}

void inputWorker(std::atomic<bool> &stopFlag, AlertManager &alertManger) {
  std::string command;
  while (!stopFlag) {
    std::getline(std::cin, command);
    if (command == "exit" || command == "quit") {
      stopFlag = true;
    } else if (command == "clear") {
      alertManger.clearAlerts();
    } else if (!command.empty()) {
      std::cout << "\nUnknown command: '" << command << "'" << std::endl;
    }
  }

  std::cout << "Input handler thread shutting down." << std::endl;
}

int main() {
  signal(SIGINT, signalHandler);

  std::cout << "SysMon Initializing..." << std::endl;

  ConfigManager configManager;
  if (!configManager.loadFromFile("monitor.conf")) {
    std::cerr
        << "Warning: Could not load 'monitor.confg'. Using default values."
        << std::endl;
  }

  double cpuThreshold = configManager.getDouble("CPU_THRESHOLD").value_or(80.0);
  double memThreshold =
      configManager.getDouble("MEMORY_THRESHOLD").value_or(75.0);
  double diskThreshold =
      configManager.getDouble("DISK_IO_THRESHOLD").value_or(5120.0); // 5MB/s

  std::cout << "CPU Alert Threshold: " << cpuThreshold << "%" << std::endl;
  std::cout << "Memory Alert Threshold: " << memThreshold << "%" << std::endl;

  DataManager dataManager;
  AlertManager alertManager;
  RuleEngine ruleEngine(dataManager, alertManager);
  UIManager uiManager(dataManager, alertManager);

  ruleEngine.addRule(Rule("CPU_Usage_Percent", Rule::Comparison::GREATER_THAN,
                          cpuThreshold, "CRITICAL: CPU Usage is high!"));
  ruleEngine.addRule(Rule("Memory_Usage_Percent",
                          Rule::Comparison::GREATER_THAN, memThreshold,
                          "CRITICAL: Memory Usage is high!"));

  std::string mainDisk = "sda";
  ruleEngine.addRule(Rule("Disk_IO_Rate_" + mainDisk,
                          Rule::Comparison::GREATER_THAN, diskThreshold,
                          "CRITICAL: Disk I/O is high on " + mainDisk));

  std::vector<std::unique_ptr<AbstractMetricCollector>> collectors;
  collectors.push_back(std::make_unique<CpuCollector>());
  collectors.push_back(std::make_unique<MemoryCollector>());
  collectors.push_back(std::make_unique<DiskCollector>(mainDisk));

  std::vector<std::thread> threads;

  std::cout << "Starting background threads" << std::endl;
  threads.emplace_back(&RuleEngine::run, &ruleEngine, std::cref(stopFlag));
  threads.emplace_back(&UIManager::run, &uiManager, std::cref(stopFlag));
  threads.emplace_back(inputWorker, std::ref(stopFlag), std::ref(alertManager));

  for (const auto &collector : collectors) {
    threads.emplace_back(collectorWorker, collector.get(),
                         std::ref(dataManager), std::cref(stopFlag), 1);
  }

  while (!stopFlag) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }

  dataManager.getConditionVariable().notify_all();

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  std::cout << "SysMon shut down successfully." << std::endl;
  return 0;
}
