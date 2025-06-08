#ifndef ABSTRACT_METRIC_COLLECTOR_H
#define ABSTRACT_METRIC_COLLECTOR_H

#include "MetricData.h"
#include <string>
#include <memory>


class AbstractMetricCollector{
	public:
		virtual ~AbstractMetricCollector() = default;
		virtual MetricData collect() = 0;
		virtual std::string getName() const  = 0;
	protected:
		AbstractMetricCollector() = default;

};

using MetricCollectorFactory = std::function<std::unique_ptr<AbstractMetricCollector>()>;

#endif

