#include "CpuCollector.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

CpuCollector::CpuCollector() : firstReadDone(false) {
	prevCpuTimes = {0 ,0 ,0 ,0 ,0 ,0 ,0 ,0}; 
}

std::string CpuCollector::getName() const{
	return "CPU_Usage";
}

bool CpuCollector::readCpuTimes(CpuTimes& times){
	std::ifstream procStatFile("/proc/stat");
	if(!procStatFile.is_open()){
		std::cerr << "ERROR: could not open /proc/stat" << std::endl;
		return false;
	}

	std::string line;
	if (std::getline(procStatFile, line)){
		if (line.rfind("cpu ", 0) == 0){
			std::istringstream iss(line.substr(4));

			if (!(iss >> times.user >> times.nice >> times.system >> times.idle>> times.iowait >> times.irq >> times.softirq >> times.steal)){
				std::cerr << "ERROR: could not parse /proc/stat" << std::endl;
				procStatFile.close();
				return false;
			}
			procStatFile.close();
			return true;
		}
	}

	procStatFile.close();
	std::cerr << "ERROR: could not found cpu line at /proc/stat" << std::endl;
	return false;
}

MetricData CpuCollector::collect(){
	CpuTimes currentCpuTimes;
	if (!readCpuTimes(currentCpuTimes)){
		return MetricData(getName(), -1.0, "Error");
	}

	double cpuPercentage = 0.0;

	if (firstReadDone) {
		long prevIdle = prevCpuTimes.getTotalIdleTime();
		long currentIdle = currentCpuTimes.getTotalIdleTime();

		long prevTotal = prevCpuTimes.getTotalTime();
		long currentTotal = currentCpuTimes.getTotalTime();

		long totalDiff = currentTotal - prevTotal;
		long idleDiff = currentIdle - prevIdle;

		if (totalDiff > 0){
			cpuPercentage = static_cast<double>(totalDiff - idleDiff) * 100.0 / static_cast<double>(totalDiff);
		}else{
			cpuPercentage = 0.0;
		}
	}else{
	firstReadDone = true;
	}

	prevCpuTimes  = currentCpuTimes; 

	return MetricData(getName(), cpuPercentage, "%");
}

































