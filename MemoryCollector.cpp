#include "MemoryCollector.h"
#include "MetricData.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

MemoryCollector::MemoryCollector() {}

std::string MemoryCollector::getName() const { return metricName; }

bool MemoryCollector::readMemInfo(std::map<std::string, long> &memInfo) {
  std::ifstream memInfoFile("/proc/meminfo");

  if (!memInfoFile.is_open()) {
    std::cerr << "ERROR: Could not open /proc/meminfo" << std::endl;
  }

  std::string line;
  while (std::getline(memInfoFile, line)) {
    std::istringstream iss(line);
    std::string key;
    long value;
    std::string unit;

    if (iss >> key >> value) {
      if (!key.empty() && key.back() == ':') {
        key.pop_back();
      }
      memInfo[key] = value;
    }
  }

  if (((memInfo.find("MemTotal") == memInfo.end())) ||
      (memInfo.find("MemAvailable") == memInfo.end())) {
    std::cerr
        << "ERROR: could not found MemTotal, MemAvailabel keys in /proc/meminfo"
        << std::endl;
    return false;
  }

  return true;
}

MetricData MemoryCollector::collect() {
  std::map<std::string, long> memInfo;
  if (!readMemInfo(memInfo)) {
    return MetricData(getName(), -1.0, "Error reading /proc/meminfo");
  }

  long memTotal = 0;
  long memAvailable = 0;

  auto itTotal = memInfo.find("MemTotal");
  if (itTotal != memInfo.end()) {
    memTotal = itTotal->second;
  } else {
    return MetricData(getName(), -2.0, "MemTotal not found");
  }

  auto itAvailable = memInfo.find("MemAvailable");
  if (itAvailable != memInfo.end()) {
    memAvailable = itAvailable->second;
  } else {
    return MetricData(getName(), -3.0, "MemAvailable not found");
  }

  double usagePercentage = 0.0;

  if (memTotal > 0) {
    long memUsed = memTotal - memAvailable;
    usagePercentage =
        static_cast<double>(memUsed) * 100.0 / static_cast<double>(memTotal);
  }

  return MetricData(getName(), usagePercentage, "%");
}
