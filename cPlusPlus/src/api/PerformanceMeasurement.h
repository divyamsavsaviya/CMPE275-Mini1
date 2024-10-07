#pragma once
#include <chrono>

class PerformanceMeasurement {
public:
    void start();
    void stop();
    double getExecutionTime() const;
    double getCPUTime() const;
    size_t getMemoryUsage() const;

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    double cpuTime;
    size_t memoryUsage;
};