#include "DiskCollector.h"
#include "MetricData.h"
#include <chrono>
#include <fstream>
#include <sstream>

DiskCollector::DiskCollector(std::string deviceName)
    : deviceName_(std::move(deviceName)), firstReadDone_(false) {}

std::string DiskCollector::getName() const {
  return "Disk_IO_Rate_" + deviceName_;
}

bool DiskCollector::readDiskStats(DiskStats &stats) {
  std::ifstream diskStatsFile("/proc/diskstats");
  if (!diskStatsFile.is_open()) {
    return false;
  }

  std::string line;
  while (std::getline(diskStatsFile, line)) {
    std::istringstream iss(line);
    // /proc/diskstats format: major minor device_name reads .... sector_read
    // ... writes ... sectors_written ....
    int major, minor;
    std::string device;

    iss >> major >> minor >> device;
    if (device == deviceName_) {
      long reads_merged, time_read,
          writes_merged; // un-used fields , added for padding
      if (iss >> stats.readsCompleted >> reads_merged >> stats.sectorsRead >>
          time_read >> stats.writesCompleted >> writes_merged >>
          stats.sectorsWritten) {
        return true;
      }
    }
  }

  return false;
}

MetricData DiskCollector::collect() {
  DiskStats currentStats;
  if (!readDiskStats(currentStats)) {
    return MetricData(getName(), -1.0, "Error or device not found");
  }

  double ioRate = 0.0; // KB/s
  auto currentTime = std::chrono::steady_clock::now();

  if (firstReadDone_) {
    // a sector is usually 512 byte
    long sectorsReadDiff = currentStats.sectorsRead - prevStats_.sectorsRead;
    long sectorsWrittenDiff =
        currentStats.sectorsWritten - prevStats_.sectorsWritten;

    long totalBytesDiff = (sectorsReadDiff + sectorsWrittenDiff) * 512;

    std::chrono::duration<double> timeDiff = currentTime - prevTimestamp_;

    if (timeDiff.count() > 0) {
      ioRate =
          (static_cast<double>(totalBytesDiff) / 1024.0) / timeDiff.count();
    }
  } else {
    firstReadDone_ = true;
  }

  prevStats_ = currentStats;
  prevTimestamp_ = currentTime;

  return MetricData(getName(), ioRate, "KB/s");
}
