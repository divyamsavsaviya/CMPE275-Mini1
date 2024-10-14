#pragma once

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "DataApi.h"
#include "PerformanceMeasurement.h"
#include "../data/ThreadPool.h"

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
    DataApi dataApi;
    ThreadPool threadPool;

    std::vector<std::unordered_map<std::string, std::string>> 
    filterData(const std::vector<std::unordered_map<std::string, std::string>>& data,
               const std::vector<std::pair<std::string, std::string>>& conditions);

    void sortData(std::vector<std::unordered_map<std::string, std::string>>& data,
                  const std::string& orderBy);

    std::vector<std::unordered_map<std::string, std::string>> 
    projectData(const std::vector<std::unordered_map<std::string, std::string>>& data,
                const std::vector<std::string>& selectColumns);

    std::vector<std::unordered_map<std::string, std::string>> 
    executeQueryParallel(const std::vector<std::string>& selectColumns,
                         const std::vector<std::pair<std::string, std::string>>& conditions,
                         const std::string& orderBy,
                         int limit);

    std::vector<std::unordered_map<std::string, std::string>> 
    processChunk(const std::vector<std::unordered_map<std::string, std::string>>& chunk,
                 const std::vector<std::string>& selectColumns,
                 const std::vector<std::pair<std::string, std::string>>& conditions);
};

void vectorized_filter(const float* data, int* result, int size, float threshold);