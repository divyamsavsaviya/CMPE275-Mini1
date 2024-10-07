#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <mpi.h>
#include "../data/CSVReader.h"
#include "PerformanceMeasurement.h"

class QueryEngine {
public:
    QueryEngine(const std::string& filename, int argc, char** argv);
    ~QueryEngine();

    std::pair<std::vector<std::unordered_map<std::string, std::string>>, PerformanceMeasurement> 
    executeQueryWithPerformance(const std::vector<std::string>& selectColumns,
                                const std::vector<std::pair<std::string, std::string>>& conditions,
                                const std::string& orderBy,
                                int limit);

private:
    std::vector<std::unordered_map<std::string, std::string>> data;
    int rank;
    int size;
    
    std::vector<std::unordered_map<std::string, std::string>> executeQueryParallel(
        const std::vector<std::string>& selectColumns,
        const std::vector<std::pair<std::string, std::string>>& conditions,
        const std::string& orderBy,
        int limit);

    std::vector<std::unordered_map<std::string, std::string>> 
    processChunk(const std::vector<std::unordered_map<std::string, std::string>>& chunk,
                 const std::vector<std::string>& selectColumns,
                 const std::vector<std::pair<std::string, std::string>>& conditions);
};