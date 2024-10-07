#include "api/PerformanceMeasurement.h"
#include "api/QueryEngine.h"
#include "api/CLI.h"
#include <iostream>

int main() {
    try {
        PerformanceMeasurement::start();
        long long initialMemory = PerformanceMeasurement::getMemoryUsage();
        std::string filename = "/mnt/c/Users/tript/IdeaProjects/CMPE275-Mini1/data/data_1.csv";
        QueryEngine queryEngine(filename);
        CLI cli(queryEngine);


        cli.run();

        PerformanceMeasurement::stop();
        long long finalMemory = PerformanceMeasurement::getMemoryUsage();

        double cpuTime = PerformanceMeasurement::getCPUTime();
        long long memoryUsed = finalMemory - initialMemory;

        std::cout << "CPU Time: " << cpuTime << " seconds" << std::endl;
        std::cout << "Memory Used: " << memoryUsed << " bytes" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
