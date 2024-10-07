#include "PerformanceMeasurement.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <sys/resource.h>
#endif

std::chrono::high_resolution_clock::time_point PerformanceMeasurement::startTime;
std::chrono::high_resolution_clock::time_point PerformanceMeasurement::endTime;

void PerformanceMeasurement::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void PerformanceMeasurement::stop() {
    endTime = std::chrono::high_resolution_clock::now();
}

double PerformanceMeasurement::getCPUTime() {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    return duration.count() / 1000000.0;
}

long long PerformanceMeasurement::getMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
#else
    long long rss = 0L;
    FILE* fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL)
        return (size_t)0L;
    if (fscanf(fp, "%*s%lld", &rss) != 1) {
        fclose(fp);
        return (size_t)0L;
    }
    fclose(fp);
    return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);
#endif
}
