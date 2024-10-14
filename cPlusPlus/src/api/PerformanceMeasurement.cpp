#include "PerformanceMeasurement.h"
#include <ctime>
#include <sys/resource.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

void PerformanceMeasurement::start() {
    std::cout << "\nStarting Performance Measurement \n";
    startTime = std::chrono::high_resolution_clock::now();
}

void PerformanceMeasurement::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    std::cout << "\nStopping Performance Measurement \n";
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    cpuTime = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec + 
              (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1e6;
    memoryUsage = usage.ru_maxrss * 1024; 
}

double PerformanceMeasurement::getExecutionTime() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1e6;
}

double PerformanceMeasurement::getCPUTime() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1e6;
}

size_t PerformanceMeasurement::getMemoryUsage() const {
    return memoryUsage;
}
