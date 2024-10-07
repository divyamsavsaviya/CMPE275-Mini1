#include "api/PerformanceMeasurement.h"
#include "api/CLI.cpp"
#include "api/QueryEngine.h"
#include <iostream>
#include <fstream>

int main() {
    PerformanceMeasurement::start();

    std::string filename = "/mnt/c/Users/tript/IdeaProjects/CMPE275-Mini1/data/data_1.csv";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: File does not exist or could not be opened: " << filename << std::endl;
        return 1;
    }
    file.close();

    QueryEngine queryEngine(filename);
    CLI cli(queryEngine);
    cli.run();

    PerformanceMeasurement::end();
    PerformanceMeasurement::getMemoryUsage();
    PerformanceMeasurement::getCPUTime();

    PerformanceMeasurement::stop();

    return 0;
}
