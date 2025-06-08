#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <variant>
#include <vector>

#include "AbstractMetricCollector.h"
#include "AlertManager.h"
#include "CpuCollector.h"
#include "DataManager.h"
#include "MemoryCollector.h"
#include "MetricData.h"
#include "Rule.h"
#include "RuleEngine.h"

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

int main() {
  signal(SIGINT, signalHandler);

  std::cout << "SysMon Initializing..." << std::endl;

  DataManager dataManager;
  AlertManager alertManager;
  RuleEngine ruleEngine(dataManager, alertManager);

  ruleEngine.addRule(Rule("CPU_Usage_Percent", Rule::Comparison::GREATER_THAN,
                          0.05, "WARNING: CPU USAGE EXCEEDED %0.05"));
  ruleEngine.addRule(Rule("Memory_Usage_Percent",
                          Rule::Comparison::GREATER_THAN, 1,
                          "WARNING: Memory USAGE EXCEEDED %1"));

  std::vector<std::unique_ptr<AbstractMetricCollector>> collectors;
  collectors.push_back(std::make_unique<CpuCollector>());
  collectors.push_back(std::make_unique<MemoryCollector>());

  std::vector<std::thread> threads;

  std::cout << "Starting threads" << std::endl;
  for (const auto &collector : collectors) {
    threads.emplace_back(collectorWorker, collector.get(),
                         std::ref(dataManager), std::cref(stopFlag), 1);
  }

  threads.emplace_back(&RuleEngine::run, &ruleEngine, std::cref(stopFlag));

  std::cout << "Main loop is running. Press Ctrl+C to exit." << std::endl;

  while (!stopFlag) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout << "\nWaiting for all threads to complete..." << std::endl;

  dataManager.getConditionVariable().notify_all();

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  std::cout << "Sysmon shutting down." << std::endl;
  return 0;
}
