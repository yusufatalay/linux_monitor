#ifndef RULE_ENGINE_H
#define RULE_ENGINE_H

#include "MetricData.h"
#include "Rule.h"
#include <atomic>
#include <string>
#include <vector>

class DataManager;
class AlertManager;

class RuleEngine {
public:
  RuleEngine(DataManager &dataManager, AlertManager &alertManager);
  ~RuleEngine() = default;

  void addRule(const Rule &rule);

  void run(const std::atomic<bool> &stopFlag);

private:
  std::vector<std::string> evaluate(const MetricData &data);

  std::vector<Rule> rules;
  DataManager &dataManager_;
  AlertManager &alertManager_;
};

#endif
