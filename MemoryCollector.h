#ifndef MEMORY_COLLECTOR_H
#define MEMORY_COLLECTOR_H

#include "AbstractMetricCollector.h"
#include <map>

class MemoryCollector : public AbstractMetricCollector {
public:
  MemoryCollector();
  ~MemoryCollector() override = default;

  MetricData collect() override;
  std::string getName() const override;

private:
  bool readMemInfo(std::map<std::string, long> &memInfo);

  const std::string metricName = "Memory_Usage_Percent";
};

#endif
