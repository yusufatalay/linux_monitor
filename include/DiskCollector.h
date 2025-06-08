#ifndef DISK_COLLECTOR_H
#define DISK_COLLECTOR_H

#include "AbstractMetricCollector.h"
#include <string>
#include <vector>

class DiskCollector : public AbstractMetricCollector {
public:
  explicit DiskCollector(std::string deviceName);
  ~DiskCollector() override = default;

  MetricData collect() override;
  std::string getName() const override;

private:
  struct DiskStats {
    long readsCompleted = 0;
    long sectorsRead = 0;
    long writesCompleted = 0;
    long sectorsWritten = 0;
  };

  bool readDiskStats(DiskStats &stats);

  std::string deviceName_;
  DiskStats prevStats_;
  std::chrono::time_point<std::chrono::steady_clock> prevTimestamp_;
  bool firstReadDone_;
};

#endif
