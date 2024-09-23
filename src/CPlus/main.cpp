#include "api/DataApi.h"
#include "api/PerformanceMeasurement.h"

int main() {
    // Start performance measurement
    PerformanceMeasurement::start();

    DataApi api("../../../data/data_1.csv");
    auto allData = api.getAllData();

    // Print the first row for testing purposes
    if (!allData.empty()) {
        allData[0].print();
    }

    // Performance measurement after operation
    PerformanceMeasurement::getMemoryUsage();
    PerformanceMeasurement::getCPUTime();

    // End performance measurement
    PerformanceMeasurement::stop();

    return 0;
}
