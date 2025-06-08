#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "MetricData.h"
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <vector>

class DataManager {
public:
  DataManager();
  ~DataManager() = default;

  void updateMetric(const MetricData &data);

  std::vector<MetricData> getLatestMetrics();

  std::mutex &getMutex();
  std::condition_variable &getConditionVariable();

private:
  std::map<std::string, MetricData> latestMetrics_;

  mutable std::mutex dataMutex_;

  std::condition_variable dataCondition_;
};

#endif
