#include "DataManager.h"
#include <condition_variable>
#include <mutex>

DataManager::DataManager() {}

void DataManager::updateMetric(const MetricData &data) {
  { // scope for mutex
    std::lock_guard<std::mutex> lock(dataMutex_);
    latestMetrics_[data.name] = data;
  } // end of scope , mutex will be freed

  dataCondition_.notify_one();
}

std::vector<MetricData> DataManager::getLatestMetrics() {
  std::lock_guard<std::mutex> lock(dataMutex_);

  std::vector<MetricData> metricsVector;

  for (const auto &pair : latestMetrics_) {
    metricsVector.push_back(pair.second);
  }

  return metricsVector;
}

std::mutex &DataManager::getMutex() { return dataMutex_; }

std::condition_variable &DataManager::getConditionVariable() {
  return dataCondition_;
}
