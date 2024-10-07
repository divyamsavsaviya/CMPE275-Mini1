#pragma once
#include <chrono>
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

class PerformanceMeasurement {
public:
    static std::chrono::high_resolution_clock::time_point startTime;
    static std::chrono::high_resolution_clock::time_point endTime;

    static void start() {
        std::cout << "Starting performance measurement..." << std::endl;
        startTime = std::chrono::high_resolution_clock::now();
    }

    static void end() {
        endTime = std::chrono::high_resolution_clock::now();
    }

    static void stop() {
        std::cout << "Performance measurement finished!" << std::endl;
    }

    static void getMemoryUsage() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        std::cout << "Memory usage: " << usage.ru_maxrss / 1024 << " MB" << std::endl;
    }

    static void getCPUTime() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        std::cout << "User CPU time: " << usage.ru_utime.tv_usec << " microseconds" << std::endl;
        std::cout << "System CPU time: " << usage.ru_stime.tv_usec << " microseconds" << std::endl;

        std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
        std::cout << "Execution time: " << elapsed.count() << " ms" << std::endl;
    }
};
