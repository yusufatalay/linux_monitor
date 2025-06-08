#ifndef CPU_COLLECTOR_H
#define CPU_COLLECTOR_H

#include "AbstractMetricCollector.h"
#include <vector>

class CpuCollector : public AbstractMetricCollector {
	public:
		CpuCollector();
		~CpuCollector() override = default;

		MetricData collect() override;
		std::string getName() const override;
	private:
		struct CpuTimes{
			long user;
			long nice;
			long system;
			long idle;
			long iowait;
			long irq;
			long softirq;
			long steal;

			long getTotalIdleTime() const {return idle + iowait;}
			long getTotalActiveTime() const {return user + nice + system + irq + softirq + steal;}
			long getTotalTime() const {return getTotalIdleTime() + getTotalActiveTime();}
		};

		bool readCpuTimes(CpuTimes& times);

		CpuTimes prevCpuTimes;
		bool firstReadDone;
};

#endif

