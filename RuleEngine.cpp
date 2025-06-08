#include "RuleEngine.h"
#include "AlertManager.h"
#include "DataManager.h"
#include "MetricData.h"
#include "Rule.h"
#include <iostream>
#include <mutex>
#include <string>
#include <variant>

RuleEngine::RuleEngine(DataManager &dataManager, AlertManager &alertManager)
    : dataManager_(dataManager), alertManager_(alertManager) {}

void RuleEngine::addRule(const Rule &rule) { rules.push_back(rule); }

void RuleEngine::run(const std::atomic<bool> &stopFlag) {
  while (!stopFlag) {
    std::unique_lock<std::mutex> lock(dataManager_.getMutex());

    dataManager_.getConditionVariable().wait(lock);

    lock.unlock();

    std::vector<MetricData> currentMetrics = dataManager_.getLatestMetrics();
    for (const auto &data : currentMetrics) {
      std::vector<std::string> triggeredAlerts = evaluate(data);
      alertManager_.processAlerts(triggeredAlerts);
    }
  }

  std::cout << "RuleEngine thread shutting down." << std::endl;
}

std::vector<std::string> RuleEngine::evaluate(const MetricData &data) {
  std::vector<std::string> triggeredAlerts;

  double metricValue = 0.0;
  bool valueExtracted = false;

  if (std::holds_alternative<double>(data.value)) {
    metricValue = std::get<double>(data.value);
    valueExtracted = true;
  } else if (std::holds_alternative<long>(data.value)) {
    metricValue = static_cast<double>(std::get<long>(data.value));
    valueExtracted = true;
  }

  if (!valueExtracted) {
    std::cerr << "RuleEngine: could not capture MetricData as double/long: "
              << data.name << std::endl;
    return triggeredAlerts;
  }

  if (data.unit == "Error reading /proc/stat" ||
      data.unit.find("Error") != std::string::npos ||
      data.unit.find("not found") != std::string::npos) {
    std::cerr << "RuleEngine: malformed metric data: " << data.name
              << " Unit: " << data.unit << std::endl;
    return triggeredAlerts;
  }

  for (const auto &rule : rules) {
    if (rule.metricName == data.name) { // is this rule for metric?
      bool ruleTriggered = false;
      switch (rule.comparisonType) {
      case Rule::Comparison::GREATER_THAN:
        if (metricValue > rule.thresholdValue) {
          ruleTriggered = true;
        }
        break;
      case Rule::Comparison::LESS_THAN:
        if (metricValue < rule.thresholdValue) {
          ruleTriggered = true;
        }
        break;
      }

      if (ruleTriggered) {
        triggeredAlerts.push_back(rule.alertMessage +
                                  " (Value: " + std::to_string(metricValue) +
                                  data.unit + ")");
      }
    }
  }
  return triggeredAlerts;
}
