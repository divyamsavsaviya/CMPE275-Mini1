#pragma once
#include <chrono>
#include <iostream>
#include <sys/resource.h>
#include <sys/time.h>

class PerformanceMeasurement {
public:
    // Function to measure execution time
    static void start() {
        std::cout << "Starting performance measurement..." << std::endl;
    }

    static void stop() {
        std::cout << "Performance measurement finished!" << std::endl;
    }

    // Function to get memory usage
    static void getMemoryUsage() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        std::cout << "Memory usage: " << usage.ru_maxrss / 1024 << " MB" << std::endl;
    }

    // Function to get CPU time
    static void getCPUTime() {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        std::cout << "User CPU time: " << usage.ru_utime.tv_sec << " seconds" << std::endl;
        std::cout << "System CPU time: " << usage.ru_stime.tv_sec << " seconds" << std::endl;
    }
};
