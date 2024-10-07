#pragma once
#include <chrono>

class PerformanceMeasurement {
public:
    static void start();
    static void stop();
    static double getCPUTime();
    static long long getMemoryUsage();

private:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point endTime;
};