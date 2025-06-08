#ifndef METRIC_DATA_H
#define METRIC_DATA_H

#include <string>
#include <chrono>
#include <variant>
#include <functional>

struct MetricData {
	std::string name;
	std::variant<double, long, std::string> value;
	std::string unit;
	std::chrono::system_clock::time_point timestamp;

	MetricData(std::string n = "", std::variant<double, long, std::string> v = 0.0, std::string u = "" ) : name(std::move(n)), value(std::move(v)), unit(std::move(u)), timestamp(std::chrono::system_clock::now()){}
};

#endif

