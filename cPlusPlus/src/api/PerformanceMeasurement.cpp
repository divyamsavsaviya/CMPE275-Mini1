#include "PerformanceMeasurement.h"
#include <ctime>
#include <sys/resource.h>

void PerformanceMeasurement::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void PerformanceMeasurement::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    cpuTime = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec + 
              (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1e6;
    memoryUsage = usage.ru_maxrss * 1024; // Convert to bytes
}

double PerformanceMeasurement::getExecutionTime() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1e6;
}

double PerformanceMeasurement::getCPUTime() const {
    return cpuTime;
}

size_t PerformanceMeasurement::getMemoryUsage() const {
    return memoryUsage;
}
