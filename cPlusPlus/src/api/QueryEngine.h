#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "DataApi.h"
#include "../data/ColumnStore.h"
#include "../data/ThreadPool.h"
#include "../data/CSVReader.h"
#include "PerformanceMeasurement.h"


class QueryEngine {
private:
    std::optional<DataApi> dataApi;
    ColumnStore dataStore;
    ThreadPool threadPool;
    std::optional<CSVReader> csvReader;

    void initializeFromFile(const std::string& filename);

public:
    QueryEngine(); // Default constructor
    QueryEngine(const std::string& filename);
    ~QueryEngine();

    std::pair<std::vector<std::unordered_map<std::string, std::string>>, PerformanceMeasurement> 
    executeQueryWithPerformance(const std::vector<std::string>& selectColumns,
                                const std::vector<std::pair<std::string, std::string>>& conditions,
                                const std::string& orderBy,
                                int limit);

    std::vector<std::unordered_map<std::string, std::string>> 
    executeQuery(const std::vector<std::string>& selectColumns,
                 const std::vector<std::pair<std::string, std::string>>& conditions,
                 const std::string& orderBy,
                 int limit);

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

    // Add these methods for MPI data transfer
    void serializeData(std::vector<char>& buffer) const;
    void deserializeData(const std::vector<char>& buffer);
};

void vectorized_filter(const float* data, int* result, int size, float threshold);
