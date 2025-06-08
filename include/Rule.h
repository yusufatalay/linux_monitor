#ifndef RULE_H
#define RULE_H

#include "MetricData.h"
#include <string>

struct Rule {
  std::string metricName;
  enum class Comparison { GREATER_THAN, LESS_THAN } comparisonType;
  double thresholdValue;
  std::string alertMessage;

  Rule(std::string mName, Comparison comp, double threshold, std::string msg)
      : metricName(std::move(mName)), comparisonType(comp),
        thresholdValue(threshold), alertMessage(std::move(msg)) {}
};

#endif
